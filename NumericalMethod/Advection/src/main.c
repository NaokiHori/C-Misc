#include "config.h"
// #include "central.h"
// #include "upwind.h"
// #include "thinc.h"
#include "acdi.h"

int main (
    void
) {
  const config_t config = {
    .length = 7.,
    .nitems = 160u,
    .velocity = 1.,
    .time_max = 3.,
  };
  const double output_freq = 1. / 30.;
  // central(&config, output_freq);
  // upwind(&config, output_freq);
  // thinc(&config, output_freq);
  acdi(&config, output_freq);
  return 0;
}
