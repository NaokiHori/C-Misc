#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define NDIMS 2

static const double PI = 3.14159265358979324;

static int get_centers(
  const size_t nitems,
  const double * const faces,
  double * const centers
) {
  for (size_t n = 0; n < nitems; n++) {
    centers[n] = 0.5 * faces[n] + 0.5 * faces[n + 1];
  }
  return 0;
}

// extract two cell-face positions used to evaluate laplacian
static int get_local_faces(
  const double * const faces,
  const size_t n,
  double * const local_faces
) {
  local_faces[0] = faces[n];
  local_faces[1] = faces[n + 1];
  return 0;
}

// extract three cell-center positions used to evaluate laplacian
static int get_local_centers(
  const double length,
  const size_t nitems,
  const double * const centers,
  const size_t n,
  double * const local_centers
) {
  local_centers[0] = 0 == n ? 0. : centers[n - 1];
  local_centers[1] = centers[n];
  local_centers[2] = nitems - 1 == n ? length : centers[n + 1];
  return 0;
}

// extract three p values used to evaluate laplacian
static int get_local_ps(
  const size_t (* const nitems)[NDIMS],
  const double * const ps,
  const size_t i,
  const size_t j,
  double (* const local_ps)[NDIMS][3]
) {
  const size_t nx = (*nitems)[0];
  const size_t ny = (*nitems)[1];
  (*local_ps)[0][0] = 0 == i ? ps[j * nx + i] : ps[j * nx + i - 1];
  (*local_ps)[0][1] = ps[j * nx + i];
  (*local_ps)[0][2] = nx - 1 == i ? ps[j * nx + i] : ps[j * nx + i + 1];
  (*local_ps)[1][0] = 0 == j ? ps[j * nx + i] : ps[(j - 1) * nx + i];
  (*local_ps)[1][1] = ps[j * nx + i];
  (*local_ps)[1][2] = ny - 1 == j ? ps[j * nx + i] : ps[(j + 1) * nx + i];
  return 0;
}

static int compute_laplace_operator(
  const double length,
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
  if (0 != get_local_centers(length, nitems, centers, i, local_centers)) {
    return 1;
  }
  coefficients[0] = 1. / (local_faces[1] - local_faces[0]) / (local_centers[1] - local_centers[0]);
  coefficients[2] = 1. / (local_faces[1] - local_faces[0]) / (local_centers[2] - local_centers[1]);
  coefficients[1] = - coefficients[0] - coefficients[2];
  return 0;
}

static int compute_residual(
  const double (* const lengths)[NDIMS],
  const size_t (* const nitems)[NDIMS],
  double * const (* const faces)[NDIMS],
  double * const (* const centers)[NDIMS],
  const double * const ps,
  const double * const qs,
  double * const residual
) {
  const double lx = (*lengths)[0];
  const double ly = (*lengths)[1];
  const size_t nx = (*nitems)[0];
  const size_t ny = (*nitems)[1];
  *residual = 0.;
  for (size_t n = 0; n < nx * ny; n++) {
    const size_t i = n % nx;
    const size_t j = n / nx;
    double laplace_operators[NDIMS][3] = {
      {0., 0., 0.},
      {0., 0., 0.},
    };
    if (0 != compute_laplace_operator(
      lx,
      nx,
      (*faces)[0],
      (*centers)[0],
      i,
      laplace_operators[0]
    )) {
      return 1;
    }
    if (0 != compute_laplace_operator(
      ly,
      ny,
      (*faces)[1],
      (*centers)[1],
      j,
      laplace_operators[1]
    )) {
      return 1;
    }
    double local_ps[NDIMS][3] = {
      {0., 0., 0.},
      {0., 0., 0.},
    };
    if (0 != get_local_ps(nitems, ps, i, j, &local_ps)) {
      return 1;
    }
    const double laplacian =
      + laplace_operators[0][0] * local_ps[0][0]
      + laplace_operators[0][1] * local_ps[0][1]
      + laplace_operators[0][2] * local_ps[0][2]
      + laplace_operators[1][0] * local_ps[1][0]
      + laplace_operators[1][1] * local_ps[1][1]
      + laplace_operators[1][2] * local_ps[1][2];
    *residual += pow(qs[n] - laplacian, 2.);
  }
  *residual = sqrt(*residual / nx / ny);
  return 0;
}

static int gauss_seidel(
  const double (* const lengths)[NDIMS],
  const size_t (* const nitems)[NDIMS],
  double * const (* const faces)[NDIMS],
  double * const (* const centers)[NDIMS],
  const double * const qs,
  double * const ps
) {
  const double lx = (*lengths)[0];
  const double ly = (*lengths)[1];
  const size_t nx = (*nitems)[0];
  const size_t ny = (*nitems)[1];
  for (size_t n = 0; n < nx * ny; n++) {
    const size_t i = n % nx;
    const size_t j = n / nx;
    double laplace_operators[NDIMS][3] = {
      {0., 0., 0.},
      {0., 0., 0.},
    };
    if (0 != compute_laplace_operator(
      lx,
      nx,
      (*faces)[0],
      (*centers)[0],
      i,
      laplace_operators[0]
    )) {
      return 1;
    }
    if (0 != compute_laplace_operator(
      ly,
      ny,
      (*faces)[1],
      (*centers)[1],
      j,
      laplace_operators[1]
    )) {
      return 1;
    }
    double local_ps[NDIMS][3] = {
      {0., 0., 0.},
      {0., 0., 0.},
    };
    if (0 != get_local_ps(nitems, ps, i, j, &local_ps)) {
      return 1;
    }
    ps[n] = 1. / (laplace_operators[0][1] + laplace_operators[1][1]) * (
      + qs[n]
      - laplace_operators[0][0] * local_ps[0][0]
      - laplace_operators[0][2] * local_ps[0][2]
      - laplace_operators[1][0] * local_ps[1][0]
      - laplace_operators[1][2] * local_ps[1][2]
    );
  }
  return 0;
}

static int solve(
  const double (* const lengths)[NDIMS],
  const size_t (* const nitems)[NDIMS],
  double * const (* const faces)[NDIMS],
  double * const (* const centers)[NDIMS],
  const double * const qs,
  double * const ps
) {
  const size_t niters = 16;
  for (size_t n = 0; n < niters; n++) {
    if (0 != gauss_seidel(lengths, nitems, faces, centers, qs, ps)) {
      return 1;
    }
  }
  return 0;
}

static int smooth(
  const double (* const lengths)[NDIMS],
  const size_t (* const nitems)[NDIMS],
  double * const (* const faces)[NDIMS],
  double * const (* const centers)[NDIMS],
  const double * const qs,
  double * const ps
) {
  const size_t niters = 4;
  for (size_t n = 0; n < niters; n++) {
    if (0 != gauss_seidel(lengths, nitems, faces, centers, qs, ps)) {
      return 1;
    }
  }
  return 0;
}

static int coarsen(
  const size_t (* const nitems)[NDIMS],
  double * const (* const faces)[NDIMS],
  double * const (* const coarse_faces)[NDIMS],
  double * const ps
) {
  const size_t nx = (*nitems)[0];
  const size_t ny = (*nitems)[1];
  const double * const x_faces = (*faces)[0];
  const double * const y_faces = (*faces)[1];
  const double * const x_coarse_faces = (*coarse_faces)[0];
  const double * const y_coarse_faces = (*coarse_faces)[1];
  for (size_t n = 0; n < nx * ny; n++) {
    const size_t i = n % nx;
    const size_t j = n / nx;
    const double weights[NDIMS][2] = {
      {
        (x_faces[2 * i + 1] - x_faces[2 * i + 0]) / (x_coarse_faces[i + 1] - x_coarse_faces[i]),
        (x_faces[2 * i + 2] - x_faces[2 * i + 1]) / (x_coarse_faces[i + 1] - x_coarse_faces[i]),
      },
      {
        (y_faces[2 * j + 1] - y_faces[2 * j + 0]) / (y_coarse_faces[j + 1] - y_coarse_faces[j]),
        (y_faces[2 * j + 2] - y_faces[2 * j + 1]) / (y_coarse_faces[j + 1] - y_coarse_faces[j]),
      },
    };
    ps[n] =
      + weights[0][0] * weights[1][0] * ps[(2 * j    ) * (2 * nx) + (2 * i    )]
      + weights[0][1] * weights[1][0] * ps[(2 * j    ) * (2 * nx) + (2 * i + 1)]
      + weights[0][0] * weights[1][1] * ps[(2 * j + 1) * (2 * nx) + (2 * i    )]
      + weights[0][1] * weights[1][1] * ps[(2 * j + 1) * (2 * nx) + (2 * i + 1)];
  }
  return 0;
}

static int refine(
  const size_t (* const nitems)[NDIMS],
  double * const ps
) {
  const size_t nx = (*nitems)[0];
  const size_t ny = (*nitems)[1];
  if (nx < 1 || ny < 1) {
    return 1;
  }
  for (size_t j_inv = 0; j_inv < ny; j_inv++) {
    const size_t j = ny - j_inv - 1;
    for (size_t i_inv = 0; i_inv < nx; i_inv++) {
      const size_t i = nx - i_inv - 1;
      const double p = ps[j * nx + i];
      ps[(2 * j + 0) * (2 * nx) + (2 * i + 0)] = p;
      ps[(2 * j + 0) * (2 * nx) + (2 * i + 1)] = p;
      ps[(2 * j + 1) * (2 * nx) + (2 * i + 0)] = p;
      ps[(2 * j + 1) * (2 * nx) + (2 * i + 1)] = p;
    }
  }
  return 0;
}

static int cycle(
  const double (* const lengths)[NDIMS],
  const size_t (* const nitems)[NDIMS],
  double * const (* const faces)[NDIMS],
  double * const (* const centers)[NDIMS],
  const double * const qs,
  double * const ps
) {
  const double lx = (*lengths)[0];
  const double ly = (*lengths)[1];
  const size_t nx = (*nitems)[0];
  const size_t ny = (*nitems)[1];
  if (nx * ny <= 16) {
    return solve(lengths, nitems, faces, centers, qs, ps);
  }
  if (0 != smooth(lengths, nitems, faces, centers, qs, ps)) {
    return 1;
  }
  if (0 != nx % 2 || 0 != ny % 2) {
    return 1;
  }
  double * const coarse_faces[NDIMS] = {
    malloc((nx / 2 + 1) * sizeof(double)),
    malloc((ny / 2 + 1) * sizeof(double)),
  };
  double * const coarse_centers[NDIMS] = {
    malloc(nx / 2 * sizeof(double)),
    malloc(ny / 2 * sizeof(double)),
  };
  for (size_t i = 0; i < nx / 2 + 1; i++) {
    coarse_faces[0][i] = (*faces)[0][2 * i];
  }
  for (size_t j = 0; j < ny / 2 + 1; j++) {
    coarse_faces[1][j] = (*faces)[1][2 * j];
  }
  if (0 != get_centers(nx / 2, coarse_faces[0], coarse_centers[0])) {
    return 1;
  }
  if (0 != get_centers(ny / 2, coarse_faces[1], coarse_centers[1])) {
    return 1;
  }
  double * const rs = malloc(nx * ny * sizeof(double));
  double * const es = malloc(nx * ny * sizeof(double));
  for (size_t n = 0; n < nx * ny; n++) {
    es[n] = 0.;
  }
  for (size_t n = 0; n < nx * ny; n++) {
    const size_t i = n % nx;
    const size_t j = n / nx;
    double laplace_operators[NDIMS][3] = {
      {0., 0., 0.},
      {0., 0., 0.},
    };
    if (0 != compute_laplace_operator(
      lx,
      nx,
      (*faces)[0],
      (*centers)[0],
      i,
      laplace_operators[0]
    )) {
      return 1;
    }
    if (0 != compute_laplace_operator(
      ly,
      ny,
      (*faces)[1],
      (*centers)[1],
      j,
      laplace_operators[1]
    )) {
      return 1;
    }
    double local_ps[NDIMS][3] = {
      {0., 0., 0.},
      {0., 0., 0.},
    };
    if (0 != get_local_ps(nitems, ps, i, j, &local_ps)) {
      return 1;
    }
    const double laplacian =
      + laplace_operators[0][0] * local_ps[0][0]
      + laplace_operators[0][1] * local_ps[0][1]
      + laplace_operators[0][2] * local_ps[0][2]
      + laplace_operators[1][0] * local_ps[1][0]
      + laplace_operators[1][1] * local_ps[1][1]
      + laplace_operators[1][2] * local_ps[1][2];
    rs[n] = qs[n] - laplacian;
  }
  const size_t coarse_nitems[NDIMS] = {
    nx / 2,
    ny / 2,
  };
  if (0 != coarsen(&coarse_nitems, faces, &coarse_faces, rs)) {
    return 1;
  }
  if (0 != cycle(lengths, &coarse_nitems, &coarse_faces, &coarse_centers, rs, es)) {
    return 1;
  }
  if (0 != refine(&coarse_nitems, es)) {
    return 1;
  }
  for (size_t n = 0; n < nx * ny; n++) {
    ps[n] += es[n];
  }
  for (size_t dim = 0; dim < NDIMS; dim++) {
    free(coarse_faces[dim]);
    free(coarse_centers[dim]);
  }
  free(rs);
  free(es);
  if (0 != smooth(lengths, nitems, faces, centers, qs, ps)) {
    return 1;
  }
  return 0;
}

static double compute_face(
    const double grad,
    const double x
) {
  return
    + (2. * grad - 2.) * (x * x * x)
    + (- 3. * grad + 3.) * (x * x)
    + grad * x;
}

int main(
    void
) {
  const double lengths[NDIMS] = {1., 1.};
  const size_t nitems[NDIMS] = {32, 32};
  double * const faces[NDIMS] = {
    malloc((nitems[0] + 1) * sizeof(double)),
    malloc((nitems[1] + 1) * sizeof(double)),
  };
  double * const centers[NDIMS] = {
    malloc(nitems[0] * sizeof(double)),
    malloc(nitems[1] * sizeof(double)),
  };
  // grid-streching indicator
  // 0: extremely stretched
  // 1: uniform
  const double grads[NDIMS] = {0.25, 0.5};
  for (size_t dim = 0; dim < NDIMS; dim++) {
    for (size_t n = 0; n < nitems[dim] + 1; n++) {
      faces[dim][n] = compute_face(grads[dim], 1. * n * lengths[dim] / nitems[dim]);
    }
    if (0 != get_centers(nitems[dim], faces[dim], centers[dim])) {
      return 1;
    }
  }
  double * const ps = malloc(nitems[0] * nitems[1] * sizeof(double));
  double * const qs = malloc(nitems[0] * nitems[1] * sizeof(double));
  for (size_t n = 0; n < nitems[0] * nitems[1]; n++) {
    const size_t i = n % nitems[0];
    const size_t j = n / nitems[0];
    const double x = centers[0][i];
    const double y = centers[1][j];
    ps[n] = 0.;
    qs[n] = 1.
      * (
        + 25. * PI * PI * cos(5. * PI * x)
        + 12. * x - 6.
      )
      * (
        + 25. * PI * PI * cos(5. * PI * y)
        + 12. * y - 6.
      );
  }
  for (size_t n_cycles = 0; ; n_cycles += 1) {
    if (0 != cycle(&lengths, &nitems, &faces, &centers, qs, ps)) {
      return 1;
    }
    double residual = 0.;
    if (0 != compute_residual(&lengths, &nitems, &faces, &centers, ps, qs, &residual)) {
      return 1;
    }
    printf("%zu % .7e\n", n_cycles, residual);
    if (residual < 1e-8) {
      break;
    }
  }
  free(ps);
  free(qs);
  for (size_t dim = 0; dim < NDIMS; dim++) {
    free(faces[dim]);
    free(centers[dim]);
  }
  return 0;
}

