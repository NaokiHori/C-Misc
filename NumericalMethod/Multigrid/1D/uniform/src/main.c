#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define PI 3.14159265358979324

static const double LENGTH = 1.;

// extract three p values used to evaluate laplacian
static int get_local_ps(
  const size_t nitems,
  const double * const ps,
  const size_t i,
  double * const local_ps
) {
  // neumann dpdx = 0
  local_ps[0] = 0 == i ? ps[0] : ps[i - 1];
  local_ps[1] = ps[i];
  // neumann dpdx = 0
  local_ps[2] = nitems - 1 == i ? ps[nitems - 1] : ps[i + 1];
  return 0;
}

static int compute_laplacian(
  const double dx,
  const double * const local_ps,
  double * const laplacian
) {
  *laplacian = (local_ps[0] - 2. * local_ps[1] + local_ps[2]) / (dx * dx);
  return 0;
}

static int compute_residual(
  const size_t nitems,
  const double * const ps,
  const double * const qs,
  double * const residual
) {
  const double dx = LENGTH / nitems;
  *residual = 0.;
  for (size_t i = 0; i < nitems; i++) {
    double local_ps[3] = {0., 0., 0.};
    if (0 != get_local_ps(nitems, ps, i, local_ps)) {
      return 1;
    }
    double laplacian = 0.;
    if (0 != compute_laplacian(dx, local_ps, &laplacian)) {
      return 1;
    }
    *residual += pow(qs[i] - laplacian, 2.);
  }
  *residual = sqrt(*residual / nitems);
  return 0;
}

static int gauss_seidel(
  const size_t nitems,
  const double * const qs,
  double * const ps
) {
  const double dx = LENGTH / nitems;
  for (size_t i = 0; i < nitems; i++) {
    double local_ps[3] = {0., 0., 0.};
    if (0 != get_local_ps(nitems, ps, i, local_ps)) {
      return 1;
    }
    ps[i] =
      + 0.5 * local_ps[0]
      + 0.5 * local_ps[2]
      - 0.5 * qs[i] * dx * dx;
  }
  return 0;
}

static int solve(
  const size_t nitems,
  const double * const qs,
  double * const ps
) {
  const size_t niters = 16;
  for (size_t n = 0; n < niters; n++) {
    if (0 != gauss_seidel(nitems, qs, ps)) {
      return 1;
    }
  }
  return 0;
}

static int smooth(
  const size_t nitems,
  const double * const qs,
  double * const ps
) {
  const size_t niters = 4;
  for (size_t n = 0; n < niters; n++) {
    if (0 != gauss_seidel(nitems, qs, ps)) {
      return 1;
    }
  }
  return 0;
}

static int coarsen(
  const size_t nitems,
  double * const ps
) {
  for (size_t i = 0; i < nitems; i++) {
    ps[i] =
      + 0.5 * ps[2 * i    ]
      + 0.5 * ps[2 * i + 1];
  }
  return 0;
}

static int refine(
  const size_t nitems,
  double * const ps
) {
  if (nitems < 1) {
    return 1;
  }
  for (size_t i = 0; i < nitems - 1; i++) {
    const size_t j = nitems - i - 1;
    ps[2 * j + 0] = ps[j];
    ps[2 * j + 1] = ps[j];
  }
  ps[1] = ps[0];
  return 0;
}

static int cycle(
  const size_t nitems,
  const double * const qs,
  double * const ps
) {
  const double dx = LENGTH / nitems;
  if (nitems <= 4) {
    return solve(nitems, qs, ps);
  }
  if (0 != smooth(nitems, qs, ps)) {
    return 1;
  }
  double * const rs = malloc(nitems * sizeof(double));
  double * const es = malloc(nitems * sizeof(double));
  for (size_t i = 0; i < nitems; i++) {
    es[i] = 0.;
  }
  for (size_t i = 0; i < nitems; i++) {
    double local_ps[3] = {0., 0., 0.};
    if (0 != get_local_ps(nitems, ps, i, local_ps)) {
      return 1;
    }
    double laplacian = 0.;
    if (0 != compute_laplacian(dx, local_ps, &laplacian)) {
      return 1;
    }
    rs[i] = qs[i] - laplacian;
  }
  if (0 != nitems % 2) {
    return 1;
  }
  if (0 != coarsen(nitems / 2, rs)) {
    return 1;
  }
  if (0 != cycle(nitems / 2, rs, es)) {
    return 1;
  }
  if (0 != refine(nitems / 2, es)) {
    return 1;
  }
  for (size_t i = 0; i < nitems; i++) {
    ps[i] += es[i];
  }
  free(rs);
  free(es);
  if (0 != smooth(nitems, qs, ps)) {
    return 1;
  }
  return 0;
}

static int output(
  const size_t nitems,
  const double * const ps
) {
  FILE * const fp = fopen("output.dat", "w");
  if (NULL == fp) {
    return 1;
  }
  const double dx = LENGTH / nitems;
  for (size_t i = 0; i < nitems; i++) {
    const double x = 0.5 * (2 * i + 1) * dx;
    fprintf(fp, "% .7e % .7e\n", x, ps[i]);
  }
  fclose(fp);
  return 0;
}

int main(
  void
) {
  const size_t nitems = 128;
  // d2p/dx2 = q
  double * const ps = malloc(nitems * sizeof(double));
  double * const qs = malloc(nitems * sizeof(double));
  const double dx = LENGTH / nitems;
  for (size_t i = 0; i < nitems; i++) {
    // defined at cell centers
    const double x = 0.5 * (2 * i + 1) * dx;
    ps[i] = 0.;
    qs[i] =
      + 25. * PI * PI * cos(5. * PI * x)
      + 12. * x - 6.;
  }
  while (1) {
    static size_t n_cycles = 0;
    if (0 != cycle(nitems, qs, ps)) {
      return 1;
    }
    n_cycles += 1;
    double residual = 0.;
    if (0 != compute_residual(nitems, ps, qs, &residual)) {
      return 1;
    }
    printf("%zu % .7e\n", n_cycles, residual);
    if (residual < 1e-8) {
      break;
    }
    if (nitems < n_cycles) {
      printf("did not converge: % .1e\n", residual);
      break;
    }
  }
  output(nitems, ps);
  free(ps);
  free(qs);
  return 0;
}

