#include <stdio.h>
#include <math.h>

static int find(
    const double ym,
    const double yp,
    const double dydtm,
    const double dydtp,
    double coefs[4]
) {
  coefs[0] = + 0.50 * ym + 0.50 * yp + 0.25 * dydtm - 0.25 * dydtp;
  coefs[1] = - 0.75 * ym + 0.75 * yp - 0.25 * dydtm - 0.25 * dydtp;
  coefs[2] =                         - 0.25 * dydtm + 0.25 * dydtp;
  coefs[3] = + 0.25 * ym - 0.25 * yp + 0.25 * dydtm + 0.25 * dydtp;
  return 0;
}

int main(
    void
) {
  const double points[][2] = {
    {5.967927102801901e-02, 9.375000000000000e-01},
    {6.250000000000000e-02, 9.430993782148759e-01},
    {9.156721198510247e-02, 9.687500000000000e-01},
    {9.375000000000000e-02, 9.704930602588197e-01},
    {9.577724903989077e-02, 9.687500000000000e-01},
    {9.750100137573856e-02, 9.375000000000000e-01},
    {9.375000000000000e-02, 9.334770621509741e-01},
    {6.250000000000000e-02, 9.326048492541303e-01},
  };
  const size_t npoints = sizeof(points) / sizeof(points[0]);
  {
    FILE * const fp = fopen("before.dat", "w");
    for (size_t n = 0; n < npoints; n++) {
      fprintf(fp, "% .7e % .7e\n", points[n][0], points[n][1]);
    }
    fclose(fp);
  }
  FILE * const fp = fopen("after.dat", "w");
  for (size_t n = 0; n < npoints; n++) {
    const double * const p0 = points[(n + 0) % npoints];
    const double * const p1 = points[(n + 1) % npoints];
    const double * const p2 = points[(n + 2) % npoints];
    const double * const p3 = points[(n + 3) % npoints];
    double xcoefs[4] = {0.};
    double ycoefs[4] = {0.};
    const double d01 = sqrt(pow(p0[0] - p1[0], 2) + pow(p0[1] - p1[1], 2));
    const double d12 = sqrt(pow(p1[0] - p2[0], 2) + pow(p1[1] - p2[1], 2));
    const double d23 = sqrt(pow(p2[0] - p3[0], 2) + pow(p2[1] - p3[1], 2));
    find(p1[0], p2[0], (p2[0] - p0[0]) / (d01 + d12) * d12 * 0.5, (p3[0] - p1[0]) / (d12 + d23) * d12 * 0.5, xcoefs);
    find(p1[1], p2[1], (p2[1] - p0[1]) / (d01 + d12) * d12 * 0.5, (p3[1] - p1[1]) / (d12 + d23) * d12 * 0.5, ycoefs);
    const size_t nsub = 16;
    for (size_t m = 0; m < nsub; m++) {
      const double t = 2. * m / (nsub - 1) - 1.;
      const double x = xcoefs[0] + t * (xcoefs[1] + t * (xcoefs[2] + t * xcoefs[3]));
      const double y = ycoefs[0] + t * (ycoefs[1] + t * (ycoefs[2] + t * ycoefs[3]));
      const double dxdt = xcoefs[1] + 2. * xcoefs[2] * t + 3. * xcoefs[3] * t * t;
      const double dydt = ycoefs[1] + 2. * ycoefs[2] * t + 3. * ycoefs[3] * t * t;
      fprintf(fp, "% .7e % .7e % .7e % .7e\n", x, y, dxdt, dydt);
    }
  }
  fclose(fp);
  return 0;
}

