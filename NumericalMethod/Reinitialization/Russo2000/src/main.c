#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "snpyio.h"

static double max_of_3(
    const double a,
    const double b,
    const double c
) {
  return fmax(a, fmax(b, c));
}

static double sign_of(
    const double a
) {
  if (a < 0.) {
    return -1.;
  } else if (0. == a) {
    return 0.;
  } else {
    return +1.;
  }
}

static int impose_bc(
    const size_t nx,
    const size_t ny,
    double * const phi
) {
  // periodic boundary condition
  for (size_t j = 0; j <= ny + 1; j++) {
    phi[j * (nx + 2) +      0] = phi[j * (nx + 2) + nx];
    phi[j * (nx + 2) + nx + 1] = phi[j * (nx + 2) +  1];
  }
  for (size_t i = 0; i <= nx + 1; i++) {
    phi[(     0) * (nx + 2) + i] = phi[ny * (nx + 2) + i];
    phi[(ny + 1) * (nx + 2) + i] = phi[ 1 * (nx + 2) + i];
  }
  return 0;
}

static int output(
    const char file_name[],
    const size_t nx,
    const size_t ny,
    const double * const buf
) {
  FILE * const fp = fopen(file_name, "w");
  if (NULL == fp) {
    puts("Failed to open file");
    return 1;
  }
  size_t header_size = 0;
  if (0 != snpyio_w_header(2, (size_t [2]) {ny, nx}, "'<f8'", false, fp, &header_size)) {
    puts("Failed to write NPY header");
    return 1;
  }
  fwrite(buf, sizeof(double), nx * ny, fp);
  fclose(fp);
  return 0;
}

int main(
    void
) {
  const size_t step_max = 1000;
  const double residual_max = 1.e-8;
  const size_t nx = 128;
  const size_t ny = 128;
  const double lx = 15.;
  const double ly = 15.;
  const double dx = lx / nx;
  const double dy = ly / ny;
  const double dt = 0.5 * fmin(dx, dy);
  double * const phi = malloc((nx + 2) * (ny + 2) * sizeof(double));
  double * const phi0 = malloc((nx + 2) * (ny + 2) * sizeof(double));
  double * const buf = malloc(nx * ny * sizeof(double));
  for (size_t j = 1; j <= ny; j++) {
    const double y = 0.5 * (2 * j - 1) * dy;
    for (size_t i = 1; i <= nx; i++) {
      const double x = 0.5 * (2 * i - 1) * dx;
      const double perturbation = 0.1 + pow(x - 0.5 * lx - 3.5, 2.) + pow(y - 0.5 * ly - 2., 2.);
      const double ellipse = sqrt(pow(0.25 * (x - 0.5 * lx), 2.) + pow(0.5 * (y - 0.5 * ly), 2.)) - 1.;
      phi[j * (nx + 2) + i] = perturbation * ellipse;
    }
  }
  impose_bc(nx, ny, phi);
  for (size_t j = 1; j <= ny; j++) {
    for (size_t i = 1; i <= nx; i++) {
      phi0[j * (nx + 2) + i] = phi[j * (nx + 2) + i];
    }
  }
  impose_bc(nx, ny, phi0);
  for (size_t j = 0; j < ny; j++) {
    for (size_t i = 0; i < nx; i++) {
      buf[j * nx + i] = phi[(j + 1) * (nx + 2) + (i + 1)];
    }
  }
  output("before.npy", nx, ny, buf);
  for (size_t step = 0; ; step++) {
    double residual = 0.;
    for (size_t j = 1; j <= ny; j++) {
      for (size_t i = 1; i <= nx; i++) {
        const double phi0_here = phi0[j * (nx + 2) + i];
        const double sign_of_phi0_here = sign_of(phi0_here);
        double src = 0.;
        if (
            0
            || phi0[(j    ) * (nx + 2) + (i - 1)] * phi0[(j    ) * (nx + 2) + (i    )] <= 0.
            || phi0[(j    ) * (nx + 2) + (i    )] * phi0[(j    ) * (nx + 2) + (i + 1)] <= 0.
            || phi0[(j - 1) * (nx + 2) + (i    )] * phi0[(j    ) * (nx + 2) + (i    )] <= 0.
            || phi0[(j    ) * (nx + 2) + (i    )] * phi0[(j + 1) * (nx + 2) + (i    )] <= 0.
        ) {
          const double dphi0dx = max_of_3(
              fabs(- 0.5 * phi0[(j    ) * (nx + 2) + (i - 1)] + 0.5 * phi0[(j    ) * (nx + 2) + (i + 1)]),
              fabs(-       phi0[(j    ) * (nx + 2) + (i    )] +       phi0[(j    ) * (nx + 2) + (i + 1)]),
              fabs(-       phi0[(j    ) * (nx + 2) + (i - 1)] +       phi0[(j    ) * (nx + 2) + (i    )])
          ) / dx;
          const double dphi0dy = max_of_3(
              fabs(- 0.5 * phi0[(j - 1) * (nx + 2) + (i    )] + 0.5 * phi0[(j + 1) * (nx + 2) + (i    )]),
              fabs(-       phi0[(j    ) * (nx + 2) + (i    )] +       phi0[(j + 1) * (nx + 2) + (i    )]),
              fabs(-       phi0[(j - 1) * (nx + 2) + (i    )] +       phi0[(j    ) * (nx + 2) + (i    )])
          ) / dy;
          const double norm = fmax(
              1e-8, // a small number to avoid zero division
              sqrt(pow(dphi0dx, 2.) + pow(dphi0dy, 2.))
          );
          src = 1. / fmin(dx, dy) * (
              - sign_of_phi0_here * fabs(phi[j * (nx + 2) + i])
              + phi0_here / norm
          );
        } else {
          double dphidx_m = (- phi[(j    ) * (nx + 2) + (i - 1)] + phi[(j    ) * (nx + 2) + (i    )]) / dx;
          double dphidx_p = (- phi[(j    ) * (nx + 2) + (i    )] + phi[(j    ) * (nx + 2) + (i + 1)]) / dx;
          double dphidy_m = (- phi[(j - 1) * (nx + 2) + (i    )] + phi[(j    ) * (nx + 2) + (i    )]) / dy;
          double dphidy_p = (- phi[(j    ) * (nx + 2) + (i    )] + phi[(j + 1) * (nx + 2) + (i    )]) / dy;
          dphidx_m = phi0_here < 0. ? fmin(0., dphidx_m) : fmax(0., dphidx_m);
          dphidx_p = phi0_here < 0. ? fmax(0., dphidx_p) : fmin(0., dphidx_p);
          dphidy_m = phi0_here < 0. ? fmin(0., dphidy_m) : fmax(0., dphidy_m);
          dphidy_p = phi0_here < 0. ? fmax(0., dphidy_p) : fmin(0., dphidy_p);
          src = - sign_of_phi0_here * (
              - 1. + sqrt(
                + fmax(pow(dphidx_m, 2.), pow(dphidx_p, 2.))
                + fmax(pow(dphidy_m, 2.), pow(dphidy_p, 2.))
              )
          );
        }
        phi[j * (nx + 2) + i] += src * dt;
        residual = fmax(residual, src);
      }
    }
    impose_bc(nx, ny, phi);
    if (fabs(residual) < residual_max || step_max < step) {
      printf("step %zu residual % .1e\n", step, residual);
      break;
    }
  }
  for (size_t j = 0; j < ny; j++) {
    for (size_t i = 0; i < nx; i++) {
      buf[j * nx + i] = phi[(j + 1) * (nx + 2) + (i + 1)];
    }
  }
  output("after.npy", nx, ny, buf);
  free(phi);
  free(phi0);
  free(buf);
  return 0;
}

