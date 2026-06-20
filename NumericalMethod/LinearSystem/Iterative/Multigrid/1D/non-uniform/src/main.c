#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define PI 3.14159265358979324

static const double LENGTH = 1.;

static int get_centers(
  const size_t nitems,
  const double * const faces,
  double * const centers
) {
  for (size_t i = 0; i < nitems; i++) {
    centers[i] = 0.5 * faces[i] + 0.5 * faces[i + 1];
  }
  return 0;
}

// extract two cell-face positions used to evaluate laplacian
static int get_local_faces(
  const double * const faces,
  const size_t i,
  double * const local_faces
) {
  local_faces[0] = faces[i];
  local_faces[1] = faces[i + 1];
  return 0;
}

// extract three cell-center positions used to evaluate laplacian
static int get_local_centers(
  const size_t nitems,
  const double * const centers,
  const size_t i,
  double * const local_centers
) {
  local_centers[0] = 0 == i ? 0. : centers[i - 1];
  local_centers[1] = centers[i];
  local_centers[2] = nitems - 1 == i ? LENGTH : centers[i + 1];
  return 0;
}

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

static int compute_laplace_operator(
  const size_t nitems,
  const double * const faces,
  const double * const centers,
  const size_t i,
  double * const coefficients
) {
  double local_faces[2] = {0., 0.};
  if (0 != get_local_faces(faces, i, local_faces)) {
    return 1;
  }
  double local_centers[3] = {0., 0., 0.};
  if (0 != get_local_centers(nitems, centers, i, local_centers)) {
    return 1;
  }
  coefficients[0] = 1. / (local_faces[1] - local_faces[0]) / (local_centers[1] - local_centers[0]);
  coefficients[2] = 1. / (local_faces[1] - local_faces[0]) / (local_centers[2] - local_centers[1]);
  coefficients[1] = - coefficients[0] - coefficients[2];
  return 0;
}

static int compute_residual(
  const size_t nitems,
  const double * const faces,
  const double * const centers,
  const double * const ps,
  const double * const qs,
  double * const residual
) {
  *residual = 0.;
  for (size_t i = 0; i < nitems; i++) {
    double laplace_operator[3] = {0., 0., 0.};
    if (0 != compute_laplace_operator(
      nitems,
      faces,
      centers,
      i,
      laplace_operator
    )) {
      return 1;
    }
    double local_ps[3] = {0., 0., 0.};
    if (0 != get_local_ps(nitems, ps, i, local_ps)) {
      return 1;
    }
    const double laplacian =
      + laplace_operator[0] * local_ps[0]
      + laplace_operator[1] * local_ps[1]
      + laplace_operator[2] * local_ps[2];
    *residual += pow(qs[i] - laplacian, 2.);
  }
  *residual = sqrt(*residual / nitems);
  return 0;
}

static int gauss_seidel(
  const size_t nitems,
  const double * const faces,
  const double * const centers,
  const double * const qs,
  double * const ps
) {
  for (size_t i = 0; i < nitems; i++) {
    double local_faces[2] = {0., 0.};
    if (0 != get_local_faces(faces, i, local_faces)) {
      return 1;
    }
    double local_centers[3] = {0., 0., 0.};
    if (0 != get_local_centers(nitems, centers, i, local_centers)) {
      return 1;
    }
    double laplace_operator[3] = {0., 0., 0.};
    if (0 != compute_laplace_operator(
      nitems,
      faces,
      centers,
      i,
      laplace_operator
    )) {
      return 1;
    }
    double local_ps[3] = {0., 0., 0.};
    if (0 != get_local_ps(nitems, ps, i, local_ps)) {
      return 1;
    }
    ps[i] = 1. / laplace_operator[1] * (
      + qs[i]
      - laplace_operator[0] * local_ps[0]
      - laplace_operator[2] * local_ps[2]
    );
  }
  return 0;
}

static int solve(
  const size_t nitems,
  const double * const faces,
  const double * const centers,
  const double * const qs,
  double * const ps
) {
  const size_t niters = 16;
  for (size_t n = 0; n < niters; n++) {
    if (0 != gauss_seidel(nitems, faces, centers, qs, ps)) {
      return 1;
    }
  }
  return 0;
}

static int smooth(
  const size_t nitems,
  const double * const faces,
  const double * const centers,
  const double * const qs,
  double * const ps
) {
  const size_t niters = 4;
  for (size_t n = 0; n < niters; n++) {
    if (0 != gauss_seidel(nitems, faces, centers, qs, ps)) {
      return 1;
    }
  }
  return 0;
}

static int coarsen(
  const size_t nitems,
  const double * const faces,
  const double * const coarse_faces,
  double * const ps
) {
  for (size_t i = 0; i < nitems; i++) {
    const double weights[2] = {
      (faces[2 * i + 1] - faces[2 * i + 0]) / (coarse_faces[i + 1] - coarse_faces[i]),
      (faces[2 * i + 2] - faces[2 * i + 1]) / (coarse_faces[i + 1] - coarse_faces[i]),
    };
    ps[i] =
      + weights[0] * ps[2 * i    ]
      + weights[1] * ps[2 * i + 1];
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
  const double * const faces,
  const double * const centers,
  const double * const qs,
  double * const ps
) {
  if (nitems <= 4) {
    return solve(nitems, faces, centers, qs, ps);
  }
  if (0 != smooth(nitems, faces, centers, qs, ps)) {
    return 1;
  }
  if (0 != nitems % 2) {
    return 1;
  }
  double * const coarse_faces = malloc((nitems / 2 + 1) * sizeof(double));
  double * const coarse_centers = malloc(nitems / 2 * sizeof(double));
  for (size_t i = 0; i < nitems / 2 + 1; i++) {
    coarse_faces[i] = faces[2 * i];
  }
  if (0 != get_centers(nitems / 2, coarse_faces, coarse_centers)) {
    return 1;
  }
  double * const rs = malloc(nitems * sizeof(double));
  double * const es = malloc(nitems * sizeof(double));
  for (size_t i = 0; i < nitems; i++) {
    es[i] = 0.;
  }
  for (size_t i = 0; i < nitems; i++) {
    double laplace_operator[3] = {0., 0., 0.};
    if (0 != compute_laplace_operator(
      nitems,
      faces,
      centers,
      i,
      laplace_operator
    )) {
      return 1;
    }
    double local_ps[3] = {0., 0., 0.};
    if (0 != get_local_ps(nitems, ps, i, local_ps)) {
      return 1;
    }
    const double laplacian =
      + laplace_operator[0] * local_ps[0]
      + laplace_operator[1] * local_ps[1]
      + laplace_operator[2] * local_ps[2];
    rs[i] = qs[i] - laplacian;
  }
  if (0 != coarsen(nitems / 2, faces, coarse_faces, rs)) {
    return 1;
  }
  if (0 != cycle(nitems / 2, coarse_faces, coarse_centers, rs, es)) {
    return 1;
  }
  if (0 != refine(nitems / 2, es)) {
    return 1;
  }
  for (size_t i = 0; i < nitems; i++) {
    ps[i] += es[i];
  }
  free(coarse_faces);
  free(coarse_centers);
  free(rs);
  free(es);
  if (0 != smooth(nitems, faces, centers, qs, ps)) {
    return 1;
  }
  return 0;
}

static int output(
  const size_t nitems,
  const double * const centers,
  const double * const ps
) {
  FILE * const fp = fopen("output.dat", "w");
  if (NULL == fp) {
    return 1;
  }
  for (size_t i = 0; i < nitems; i++) {
    fprintf(fp, "% .7e % .7e\n", centers[i], ps[i]);
  }
  fclose(fp);
  return 0;
}

int main(
    void
) {
  const size_t nitems = 1 << 6;
  double * const faces = malloc((nitems + 1) * sizeof(double));
  double * const centers = malloc(nitems * sizeof(double));
  for (size_t i = 0; i < nitems + 1; i++) {
    // grid-streching indicator
    // 0: extremely stretched
    // 1: uniform
    const double grad = 0.25;
    const double x = 1. * i * LENGTH / nitems;
    faces[i] =
      + (2. * grad - 2.) * (x * x * x)
      + (- 3. * grad + 3.) * (x * x)
      + grad * x;
  }
  if (0 != get_centers(nitems, faces, centers)) {
    return 1;
  }
  double * const ps = malloc(nitems * sizeof(double));
  double * const qs = malloc(nitems * sizeof(double));
  for (size_t i = 0; i < nitems; i++) {
    const double x = centers[i];
    ps[i] = 0.;
    qs[i] =
      + 25. * PI * PI * cos(5. * PI * x)
      + 12. * x - 6.;
  }
  while (1) {
    static size_t n_cycles = 0;
    if (0 != cycle(nitems, faces, centers, qs, ps)) {
      return 1;
    }
    n_cycles += 1;
    double residual = 0.;
    if (0 != compute_residual(nitems, faces, centers, ps, qs, &residual)) {
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
  output(nitems, centers, ps);
  free(ps);
  free(qs);
  free(faces);
  free(centers);
  return 0;
}

