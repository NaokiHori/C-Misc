#if !defined(COLOR_CONVERTER_H)
#define COLOR_CONVERTER_H

typedef struct {
  int (* const hsv_to_rgb)(
      const double hsv[3],
      double rgb[3]
  );
  int (* const rgb_to_hsv)(
      const double rgb[3],
      double hsv[3]
  );
  int (* const hsl_to_rgb)(
      const double hsl[3],
      double rgb[3]
  );
  int (* const rgb_to_hsl)(
      const double rgb[3],
      double hsl[3]
  );
} color_converter_t;

extern const color_converter_t color_converter;

#endif // COLOR_CONVERTER_H
