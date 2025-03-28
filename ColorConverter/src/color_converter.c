#include <math.h>
#include "color_converter.h"

// convert HSV value [0:1] to RGB value [0:1]
// ref: https://github.com/python/cpython/blob/3.11/Lib/colorsys.py
static int hsv_to_rgb(
    const double hsv[3],
    double rgb[3]
) {
  const double h = hsv[0];
  const double s = hsv[1];
  const double v = hsv[2];
  if (0. == s) {
    rgb[0] = v;
    rgb[1] = v;
    rgb[2] = v;
    return 0;
  }
  const double f = 6. * h - floor(6. * h);
  const double p = v * (1. - s);
  const double q = v * (1. - s * f);
  const double t = v * (1. - s * (1. - f));
  if (h < 1. / 6.) {
    rgb[0] = v;
    rgb[1] = t;
    rgb[2] = p;
  } else if (h < 2. / 6.) {
    rgb[0] = q;
    rgb[1] = v;
    rgb[2] = p;
  } else if (h < 3. / 6.) {
    rgb[0] = p;
    rgb[1] = v;
    rgb[2] = t;
  } else if (h < 4. / 6.) {
    rgb[0] = p;
    rgb[1] = q;
    rgb[2] = v;
  } else if (h < 5. / 6.) {
    rgb[0] = t;
    rgb[1] = p;
    rgb[2] = v;
  } else {
    rgb[0] = v;
    rgb[1] = p;
    rgb[2] = q;
  }
  return 0;
}

// convert RGB value [0:1] to HSV value [0:1]
// ref: https://github.com/python/cpython/blob/3.11/Lib/colorsys.py
static int rgb_to_hsv(
    const double rgb[3],
    double hsv[3]
) {
  const double r = rgb[0];
  const double g = rgb[1];
  const double b = rgb[2];
  const double min = fmin(r, fmin(g, b));
  const double max = fmax(r, fmax(g, b));
  if (min == max) {
    hsv[0] = 0.;
  } else if (r == max) {
    const double h = (g - b) / (max - min) / 6.;
    hsv[0] = h + (h < 0 ? 1. : 0.);
  } else if (g == max) {
    const double h = (b - r) / (max - min) / 6. + 1. / 3.;
    hsv[0] = h + (h < 0 ? 1. : 0.);
  } else {
    const double h = (r - g) / (max - min) / 6. + 2. / 3.;
    hsv[0] = h + (h < 0 ? 1. : 0.);
  }
  hsv[1] = 0. < max ? (max - min) / max : 0.;
  hsv[2] = max;
  return 0;
}

// convert HSL value [0:1] to RGB value [0:1]
// https://www.peko-step.com/tool/hslrgb.html#google_vignette
static int hsl_to_rgb(
    const double hsl[3],
    double rgb[3]
) {
  const double h = hsl[0];
  const double s = hsl[1];
  const double l = hsl[2];
  const double min = l < 0.5 ? l - l * s : l - (1. - l) * s;
  const double max = l < 0.5 ? l + l * s : l + (1. - l) * s;
  if (h < 1. / 6.) {
    rgb[0] = max;
    rgb[1] = (6. * h) * (max - min) + min;
    rgb[2] = min;
  } else if (h < 2. / 6.) {
    rgb[0] = (2. - 6. * h) * (max - min) + min;
    rgb[1] = max;
    rgb[2] = min;
  } else if (h < 3. / 6.) {
    rgb[0] = min;
    rgb[1] = max;
    rgb[2] = (6. * h - 2.) * (max - min) + min;
  } else if (h < 4. / 6.) {
    rgb[0] = min;
    rgb[1] = (4. - 6. * h) * (max - min) + min;
    rgb[2] = max;
  } else if (h < 5. / 6.) {
    rgb[0] = (6. * h - 4.) * (max - min) + min;
    rgb[1] = min;
    rgb[2] = max;
  } else {
    rgb[0] = max;
    rgb[1] = min;
    rgb[2] = (6. - 6. * h) * (max - min) + min;
  }
  return 0;
}

// convert HSL value [0:1] to RGB value [0:1]
// https://www.peko-step.com/tool/hslrgb.html#google_vignette
static int rgb_to_hsl(
    const double rgb[3],
    double hsl[3]
) {
  const double r = rgb[0];
  const double g = rgb[1];
  const double b = rgb[2];
  const double min = fmin(r, fmin(g, b));
  const double max = fmax(r, fmax(g, b));
  // hue
  if (min == max) {
    hsl[0] = 0.;
  } else {
    if (r == max) {
      hsl[0] = 1. / 6. * (g - b) / (max - min);
    } else if (g == max) {
      hsl[0] = 1. / 6. * (b - r) / (max - min) + 2. / 6.;
    } else {
      hsl[0] = 1. / 6. * (r - g) / (max - min) + 4. / 6.;
    }
  }
  if (hsl[0] < 0.) {
    hsl[0] += 1.;
  }
  // saturation
  const double cnt = 0.5 * min + 0.5 * max;
  if (min == max) {
    hsl[1] = 0.;
  } else {
    if (cnt < 0.5) {
      hsl[1] = (cnt - min) / cnt;
    } else {
      hsl[1] = (max - cnt) / (1. - cnt);
    }
  }
  // lightness
  hsl[2] = cnt;
  return 0;
}

const color_converter_t color_converter = {
  .hsv_to_rgb = hsv_to_rgb,
  .rgb_to_hsv = rgb_to_hsv,
  .hsl_to_rgb = hsl_to_rgb,
  .rgb_to_hsl = rgb_to_hsl,
};

