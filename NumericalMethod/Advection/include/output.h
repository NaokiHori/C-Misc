#if !defined(OUTPUT_H)
#define OUTPUT_H

typedef struct {
  const double freq;
  double next;
  size_t cntr;
} output_config_t;

extern int output (
    const config_t * const config,
    output_config_t * const output_config,
    const char * prefix,
    const double time,
    const double * const x,
    const double * const vof
);

#endif // OUTPUT_H
