#include <math.h>
#include <stddef.h>
#include <stdio.h>

static double compute_dpdt(
    const double p
) {
  // right-hand side of the autonomous ODE:
  //   dp/dt = q
  // here we adopt
  //   p = - log(1 - t)
  //   dp/dt = exp(p)
  return exp(p);
}

static double compute_p(
    const double t
) {
  // answer of the autonomous ODE:
  //   dp/dt = q
  // here we adopt
  //   p = - log(1 - t)
  //   dp/dt = exp(p)
  return - log(1. - t);
}

static double integrate(
    const double dt,
    double p
) {
  // Low-storage three-step Runge-Kutta scheme by Williamson 1980
#define NUM_STAGES 3
  const double betas[NUM_STAGES]  = {
         0. / 5760.,
    - 3200. / 5760.,
    - 6885. / 5760.,
  };
  const double gammas[NUM_STAGES] = {
    + 1920. / 5760.,
    + 5400. / 5760.,
    + 3072. / 5760.,
  };
  double r = 0.;
  for (size_t stage = 0; stage < NUM_STAGES; stage++) {
    r = compute_dpdt(p) + betas[stage] * r;
    p += gammas[stage] * r * dt;
  }
  return p;
#undef NUM_STAGES
}

int main(
    void
) {
  const size_t num_steps_list[] = {4, 8, 16, 32, 64, 128, 256};
  for (size_t n = 0; n < sizeof(num_steps_list) / sizeof(num_steps_list[0]); n++) {
    const size_t num_steps = num_steps_list[n];
    struct {
      double start;
      double finish;
      double now;
    } time = {
      .start = 0.,
      .finish = 0.5,
      .now = 0.,
    };
    const double time_step_size = (time.finish - time.start) / num_steps;
    double p = 0.;
    double error = 0.;
    for (size_t step = 0; step < num_steps; step++) {
      p = integrate(time_step_size, p);
      time.now += time_step_size;
      error += pow(compute_p(time.now) - p, 2.);
    }
    printf("%zu % .7e\n", num_steps, sqrt(error / num_steps));
  }
  return 0;
}

