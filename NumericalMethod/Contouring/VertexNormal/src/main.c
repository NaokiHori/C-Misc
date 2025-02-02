#include <stdio.h>
#include <math.h>

typedef struct {
  double x;
  double y;
} point_t;

int main(
    void
) {
  const point_t points[] = {
    {.x = 5.967927102801901e-02, .y = 9.375000000000000e-01},
    {.x = 6.250000000000000e-02, .y = 9.430993782148759e-01},
    {.x = 9.156721198510247e-02, .y = 9.687500000000000e-01},
    {.x = 9.375000000000000e-02, .y = 9.704930602588197e-01},
    {.x = 9.577724903989077e-02, .y = 9.687500000000000e-01},
    {.x = 9.750100137573856e-02, .y = 9.375000000000000e-01},
    {.x = 9.375000000000000e-02, .y = 9.334770621509741e-01},
    {.x = 6.250000000000000e-02, .y = 9.326048492541303e-01},
  };
  const size_t npoints = sizeof(points) / sizeof(points[0]);
  FILE * const fp = fopen("result.dat", "w");
  for (size_t n = 0; n < npoints; n++) {
    if (0 == n) {
      const point_t * const point_0 = points + n;
      const point_t * const point_p = points + n + 1;
      const double d = sqrt(
          + pow(point_p->x - point_0->x, 2)
          + pow(point_p->y - point_0->y, 2)
      );
      const double dxdt = (point_p->x - point_0->x) / d;
      const double dydt = (point_p->y - point_0->y) / d;
      fprintf(fp, "% .7e % .7e % .7e % .7e\n", point_0->x, point_0->y, dxdt, dydt);
    } else if (npoints - 1 == n) {
      const point_t * const point_m = points + n - 1;
      const point_t * const point_0 = points + n;
      const double d = sqrt(
          + pow(point_m->x - point_0->x, 2)
          + pow(point_m->y - point_0->y, 2)
      );
      const double dxdt = (point_0->x - point_m->x) / d;
      const double dydt = (point_0->y - point_m->y) / d;
      fprintf(fp, "% .7e % .7e % .7e % .7e\n", point_0->x, point_0->y, dxdt, dydt);
    } else {
      const point_t * const point_m = points + n - 1;
      const point_t * const point_0 = points + n    ;
      const point_t * const point_p = points + n + 1;
      const double d_m = sqrt(
          + pow(point_m->x - point_0->x, 2)
          + pow(point_m->y - point_0->y, 2)
      );
      const double d_p = sqrt(
          + pow(point_p->x - point_0->x, 2)
          + pow(point_p->y - point_0->y, 2)
      );
      const double dxdt = 1. / (d_m + d_p) * (
          + d_m / d_p * (point_p->x - point_0->x)
          - d_p / d_m * (point_m->x - point_0->x)
      );
      const double dydt = 1. / (d_m + d_p) * (
          + d_m / d_p * (point_p->y - point_0->y)
          - d_p / d_m * (point_m->y - point_0->y)
      );
      fprintf(fp, "% .7e % .7e % .7e % .7e\n", point_0->x, point_0->y, dxdt, dydt);
    }
  }
  fclose(fp);
  return 0;
}

