#if !defined(CONFIG_H)
#define CONFIG_H

#include <stddef.h> // size_t

typedef struct {
  const double length;
  const size_t nitems;
  const double velocity;
  const double time_max;
} config_t;

extern double compute_dx (
    const config_t * const config
);

extern double decide_dt (
    const config_t * const config
);

#endif // CONFIG_H
