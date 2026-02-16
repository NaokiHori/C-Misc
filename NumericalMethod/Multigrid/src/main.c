#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define PI 3.14159265358979324

static const double L = 1.;

static int compute_residual(
  const size_t nitems,
  const double * const p,
  const double * const q,
  double * const residual
) {
  const double dx = L / nitems;
  *residual = 0.;
  for (size_t i = 0; i < nitems; i++) {
    // neumann dpdx = 0
    const double pm = 0 == i ? p[0] : p[i - 1];
    const double pc = p[i];
    // neumann dpdx = 0
    const double pp = nitems - 1 == i ? p[nitems - 1] : p[i + 1];
    const double qc = q[i];
    *residual += pow(qc - (pm - 2. * pc + pp) / (dx * dx), 2.);
  }
  *residual = sqrt(*residual / nitems);
  return 0;
}

static int gauss_seidel(
  const size_t nitems,
  const double * const q,
  double * const p
) {
  const double dx = L / nitems;
  for (size_t i = 0; i < nitems; i++) {
    // neumann dpdx = 0
    const double pm = 0 == i ? p[0] : p[i - 1];
    double * const pc = p + i;
    // neumann dpdx = 0
    const double pp = nitems - 1 == i ? p[nitems - 1] : p[i + 1];
    const double qc = q[i];
    *pc =
      + 0.5 * pm
      + 0.5 * pp
      - 0.5 * qc * dx * dx;
  }
  return 0;
}

static int solve(
  const size_t nitems,
  const double * const q,
  double * const p
) {
  const size_t niters = 16;
  for (size_t n = 0; n < niters; n++) {
    if (0 != gauss_seidel(nitems, q, p)) {
      return 1;
    }
  }
  return 0;
}

static int smooth(
  const size_t nitems,
  const double * const q,
  double * const p
) {
  const size_t niters = 4;
  for (size_t n = 0; n < niters; n++) {
    if (0 != gauss_seidel(nitems, q, p)) {
      return 1;
    }
  }
  return 0;
}

static int coarsen(
  const size_t nitems,
  double * const p
) {
  for (size_t i = 0; i < nitems; i++) {
    p[i] =
      + 0.5 * p[2 * i    ]
      + 0.5 * p[2 * i + 1];
  }
  return 0;
}

static int refine(
  const size_t nitems,
  double * const p
) {
  if (nitems < 1) {
    return 1;
  }
  for (size_t i = 0; i < nitems - 1; i++) {
    const size_t j = nitems - i - 1;
    p[2 * j + 0] = p[j];
    p[2 * j + 1] = p[j];
  }
  p[1] = p[0];
  return 0;
}

static int cycle(
  const size_t nitems,
  const double * const q,
  double * const p
) {
  const double dx = L / nitems;
  if (nitems <= 4) {
    return solve(nitems, q, p);
  }
  if (0 != smooth(nitems, q, p)) {
    return 1;
  }
  double * const r = malloc(nitems * sizeof(double));
  double * const e = malloc(nitems * sizeof(double));
  for (size_t i = 0; i < nitems; i++) {
    e[i] = 0.;
  }
  for (size_t i = 0; i < nitems; i++) {
    const double pm = 0 == i ? p[0] : p[i - 1];
    const double pc = p[i];
    const double pp = nitems - 1 == i ? p[nitems - 1] : p[i + 1];
    const double qc = q[i];
    double * const rc = r + i;
    *rc = qc - (pm - 2. * pc + pp) / (dx * dx);
  }
  if (0 != nitems % 2) {
    return 1;
  }
  if (0 != coarsen(nitems / 2, r)) {
    return 1;
  }
  if (0 != cycle(nitems / 2, r, e)) {
    return 1;
  }
  if (0 != refine(nitems / 2, e)) {
    return 1;
  }
  for (size_t i = 0; i < nitems; i++) {
    p[i] += e[i];
  }
  free(r);
  free(e);
  if (0 != smooth(nitems, q, p)) {
    return 1;
  }
  return 0;
}

static int output(
  const size_t nitems,
  const double * const p
) {
  FILE * const fp = fopen("output.dat", "w");
  if (NULL == fp) {
    return 1;
  }
  const double dx = L / nitems;
  for (size_t i = 0; i < nitems; i++) {
    const double x = 0.5 * (2 * i + 1) * dx;
    fprintf(fp, "% .7e % .7e\n", x, p[i]);
  }
  fclose(fp);
  return 0;
}

int main(
  void
) {
  const size_t nitems = 128;
  double * const p = malloc(nitems * sizeof(double));
  double * const q = malloc(nitems * sizeof(double));
  const double dx = L / nitems;
  for (size_t i = 0; i < nitems; i++) {
    const double x = 0.5 * (2 * i + 1) * dx;
    p[i] = 0.;
    // answer[i] =
    //   + C // arbitrary constant
    //   - cos(5. * PI * x)
    //   + 2. * pow(x, 3.) - 3. * pow(x, 2.);
    q[i] =
      + 25. * PI * PI * cos(5. * PI * x)
      + 12. * x - 6.;
  }
  while (1) {
    static size_t n_cycles = 0;
    if (0 != cycle(nitems, q, p)) {
      return 1;
    }
    n_cycles += 1;
    double residual = 0.;
    if (0 != compute_residual(nitems, p, q, &residual)) {
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
  output(nitems, p);
  free(p);
  free(q);
  return 0;
}

