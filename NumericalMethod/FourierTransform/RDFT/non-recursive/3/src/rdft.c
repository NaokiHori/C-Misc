#include <complex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "rdft.h"
#include "./dft.h"

static const double pi = 3.1415926535897932384626;

typedef struct {
  size_t i;
  size_t j;
} swap_list_item_t;

struct rdft_plan_t {
  // degrees of freedom
  size_t nitems;
  // complex DFT
  dft_plan_t * dft_plan;
  // pre-computed trigonometric values
  double complex * twiddle_factors;
  // to reorder input / output
  size_t nswap_list_items;
  swap_list_item_t * swap_list_items;
};

static void * memory_alloc(
    const size_t size
) {
  void * const ptr = malloc(size);
  if (NULL == ptr) {
    fprintf(
        stderr,
        "[FATAL %s:%d] failed to allocate %zu bytes\n",
        __FILE__,
        __LINE__,
        size
    );
    exit(EXIT_FAILURE);
  }
  return ptr;
}

static void memory_free(
    void * const ptr
) {
  free(ptr);
}

static int construct_swap_list(
    const size_t nitems,
    size_t * const nswap_list_items,
    swap_list_item_t * const swap_list_items
) {
  const size_t nitems_half = nitems / 2;
  *nswap_list_items = 0;
  size_t * const ordered = memory_alloc(nitems * sizeof(size_t));
  size_t * const expected = memory_alloc(nitems * sizeof(size_t));
  for (size_t n = 0; n < nitems; n++) {
    ordered[n] = n;
    expected[n] = n < nitems_half ? 2 * n : 2 * (n - nitems_half) + 1;
  }
  for (size_t i = 0; i < nitems; i++) {
    if (ordered[i] == expected[i]) {
      continue;
    }
    for (size_t j = i + 1; j < nitems; j++) {
      if (ordered[j] == expected[i]) {
        const size_t tmp = ordered[i];
        ordered[i] = ordered[j];
        ordered[j] = tmp;
        swap_list_item_t * const swap_list_item = swap_list_items + *nswap_list_items;
        swap_list_item->i = i;
        swap_list_item->j = j;
        *nswap_list_items += 1;
        break;
      }
    }
  }
  memory_free(ordered);
  memory_free(expected);
  return 0;
}

static int apply_swap_list(
    const bool is_backward,
    const size_t nswap_list_items,
    const swap_list_item_t * const swap_list_items,
    double * const values
) {
  for (size_t n = 0; n < nswap_list_items; n++) {
    const swap_list_item_t * const swap_list_item = swap_list_items
      + (is_backward ? nswap_list_items - n - 1 : n);
    const size_t i = swap_list_item->i;
    const size_t j = swap_list_item->j;
    const double tmp = values[i];
    values[i] = values[j];
    values[j] = tmp;
  }
  return 0;
}

int rdft_execute_inplace_forward_transform(
    const rdft_plan_t * const plan,
    double * const values
) {
  const size_t nitems = plan->nitems;
  const size_t nitems_half = nitems / 2;
  const double complex * const twiddle_factors = plan->twiddle_factors;
  apply_swap_list(
      false,
      plan->nswap_list_items,
      plan->swap_list_items,
      values
  );
  double * const reals = values;
  double * const imags = values + nitems_half;
  dft_execute_inplace_forward_transform(plan->dft_plan, reals, imags);
  {
    const double even = reals[0];
    const double odd = imags[0];
    reals[0] = even + odd;
    imags[0] = even - odd;
  }
  for (size_t n = 1; n < (nitems_half + 1) / 2; n++) {
    double * const local_reals[2] = {
      reals + n,
      reals + nitems_half - n,
    };
    double * const local_imags[2] = {
      imags + n,
      imags + nitems_half - n,
    };
    const double complex z = *local_reals[0] + *local_imags[0] * I;
    const double complex z_mirror = *local_reals[1] + *local_imags[1] * I;
    const double complex even = 0.5 * (z + conj(z_mirror));
    const double complex odd = - 0.5 * (z - conj(z_mirror)) * I;
    const double complex twiddle = twiddle_factors[n];
    const double complex x = even + twiddle * odd;
    const double complex x_mirror = conj(even - twiddle * odd);
    *local_reals[0] = creal(x);
    *local_reals[1] = creal(x_mirror);
    *local_imags[0] = - cimag(x_mirror);
    *local_imags[1] = - cimag(x);
  }
  return 0;
}

int rdft_execute_inplace_backward_transform(
    const rdft_plan_t * const plan,
    double * const values
) {
  const size_t nitems = plan->nitems;
  const size_t nitems_half = nitems / 2;
  const double complex * const twiddle_factors = plan->twiddle_factors;
  double * const reals = values;
  double * const imags = values + nitems_half;
  {
    const double x = reals[0];
    const double x_nyquist = imags[0];
    reals[0] = 0.5 * (x + x_nyquist);
    imags[0] = 0.5 * (x - x_nyquist);
  }
  for (size_t n = 1; n < (nitems_half + 1) / 2; n++) {
    double * const local_reals[2] = {
      reals + n,
      reals + nitems_half - n,
    };
    double * const local_imags[2] = {
      imags + n,
      imags + nitems_half - n,
    };
    const double complex x = *local_reals[0] - *local_imags[1] * I;
    const double complex x_mirror = *local_reals[1] - *local_imags[0] * I;
    const double complex twiddle = twiddle_factors[n];
    const double complex even = 0.5 * (x + conj(x_mirror));
    const double complex odd = 0.5 * (x - conj(x_mirror)) * conj(twiddle);
    const double complex z = even + odd * I;
    const double complex z_mirror = conj(even - odd * I);
    *local_reals[0] = creal(z);
    *local_reals[1] = creal(z_mirror);
    *local_imags[0] = cimag(z);
    *local_imags[1] = cimag(z_mirror);
  }
  dft_execute_inplace_backward_transform(plan->dft_plan, reals, imags);
  apply_swap_list(
      true,
      plan->nswap_list_items,
      plan->swap_list_items,
      values
  );
  return 0;
}

int rdft_execute_outofplace_forward_transform(
    const rdft_plan_t * const plan,
    const double * const restrict inputs,
    double * const restrict outputs
) {
  const size_t nitems = plan->nitems;
  for (size_t n = 0; n < nitems; n++) {
    outputs[n] = inputs[n];
  }
  rdft_execute_inplace_forward_transform(plan, outputs);
  return 0;
}

int rdft_execute_outofplace_backward_transform(
    const rdft_plan_t * const plan,
    const double * const restrict inputs,
    double * const restrict outputs
) {
  const size_t nitems = plan->nitems;
  for (size_t n = 0; n < nitems; n++) {
    outputs[n] = inputs[n];
  }
  rdft_execute_inplace_backward_transform(plan, outputs);
  return 0;
}

int rdft_init_plan(
    const size_t nitems,
    rdft_plan_t ** const plan
) {
  if (0 != nitems % 2) {
    fprintf(stderr, "nitems should be even\n");
    *plan = NULL;
    return 1;
  }
  *plan = memory_alloc(sizeof(rdft_plan_t));
  (*plan)->nitems = nitems;
  (*plan)->twiddle_factors = NULL;
  (*plan)->swap_list_items = NULL;
  if (0 != dft_init_plan(nitems / 2, &(*plan)->dft_plan)) {
    fprintf(stderr, "failed to create dft plan\n");
    rdft_destroy_plan(plan);
    return 1;
  }
  // prepare swap list
  // NOTE: at most nitems entries exist
  size_t * const nswap_list_items = &(*plan)->nswap_list_items;
  swap_list_item_t ** const swap_list_items = &(*plan)->swap_list_items;
  *swap_list_items = memory_alloc(nitems * sizeof(swap_list_item_t));
  if (0 != construct_swap_list(
      nitems,
      nswap_list_items,
      *swap_list_items
  )) {
    fprintf(stderr, "failed to construct swap list\n");
    rdft_destroy_plan(plan);
    return 1;
  }
  // prepare trigonometric table
  const size_t ntwiddle_factors = ((nitems / 2) + 1) / 2;
  double complex ** const twiddle_factors = &(*plan)->twiddle_factors;
  *twiddle_factors = memory_alloc(ntwiddle_factors * sizeof(double complex));
  for (size_t n = 0; n < ntwiddle_factors; n++) {
    (*twiddle_factors)[n] = cexp(- 2. * pi * n / nitems * I);
  }
  return 0;
}

int rdft_destroy_plan(
    rdft_plan_t ** const plan
) {
  if (NULL == *plan) {
    return 0;
  }
  dft_destroy_plan(&(*plan)->dft_plan);
  memory_free((*plan)->twiddle_factors);
  memory_free((*plan)->swap_list_items);
  memory_free(*plan);
  *plan = NULL;
  return 0;
}

