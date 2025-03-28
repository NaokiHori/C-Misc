#include <stdio.h>
#include "color_converter.h"

static int hsv_to_rgb(
    void
) {
  const double hsv[3] = {1. / 3., 1., 0.5};
  double rgb[3] = {0., 0., 0.};
  color_converter.hsv_to_rgb(hsv, rgb);
  printf(
      "hsv (% .3f % .3f % .3f) -> rgb (% .3f % .3f % .3f)\n",
      hsv[0], hsv[1], hsv[2],
      rgb[0], rgb[1], rgb[2]
  );
  return 0;
}

static int rgb_to_hsv(
    void
) {
  const double rgb[3] = {0., 0.5, 0.};
  double hsv[3] = {0., 0., 0.};
  color_converter.rgb_to_hsv(rgb, hsv);
  printf(
      "rgb (% .3f % .3f % .3f) -> hsv (% .3f % .3f % .3f)\n",
      rgb[0], rgb[1], rgb[2],
      hsv[0], hsv[1], hsv[2]
  );
  return 0;
}

static int hsl_to_rgb(
    void
) {
  const double hsl[3] = {0., 1., 0.5};
  double rgb[3] = {0., 0., 0.};
  color_converter.hsl_to_rgb(hsl, rgb);
  printf(
      "hsl (% .3f % .3f % .3f) -> rgb (% .3f % .3f % .3f)\n",
      hsl[0], hsl[1], hsl[2],
      rgb[0], rgb[1], rgb[2]
  );
  return 0;
}

static int rgb_to_hsl(
    void
) {
  const double rgb[3] = {1., 0., 0.};
  double hsl[3] = {0., 0., 0.};
  color_converter.rgb_to_hsl(rgb, hsl);
  printf(
      "rgb (% .3f % .3f % .3f) -> hsl (% .3f % .3f % .3f)\n",
      rgb[0], rgb[1], rgb[2],
      hsl[0], hsl[1], hsl[2]
  );
  return 0;
}

int main(
    void
) {
  hsv_to_rgb();
  rgb_to_hsv();
  hsl_to_rgb();
  rgb_to_hsl();
  return 0;
}

