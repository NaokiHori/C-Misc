#include <limits.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "dft.h"

#define RADIX_MAX 13

static const double pi = 3.14159265358979324;

typedef struct {
  double cos;
  double sin;
} trig_t;

typedef struct {
  size_t i;
  size_t j;
} swap_list_item_t;

struct dft_plan_t {
  // degrees of freedom and its prime factorization
  size_t nitems;
  size_t nfactors;
  size_t * factors;
  // pre-computed trigonometric values
  trig_t * trigs;
  // reorder input buffer
  size_t nswap_list;
  swap_list_item_t * swap_list;
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

static int check_and_push_factor(
    const size_t new_factor,
    size_t * const nfactors,
    size_t * const factors
) {
  if (RADIX_MAX < new_factor) {
    fprintf(stderr, "prime number is not supported: %zu\n", new_factor);
    return 1;
  }
  factors[*nfactors] = new_factor;
  *nfactors += 1;
  return 0;
}

static int prime_factorize(
    const size_t nitems,
    size_t * const nfactors,
    size_t * const factors
) {
  *nfactors = 0;
  size_t n = nitems;
  size_t d = 2;
  while (d * d <= n) {
    while (0 == n % d) {
      if (0 != check_and_push_factor(d, nfactors, factors)) {
        return 1;
      }
      n /= d;
    }
    d += 1;
  }
  if (1 < n) {
    if (0 != check_and_push_factor(n, nfactors, factors)) {
      return 1;
    }
  }
  return 0;
}

static int rearrange_indices(
    const size_t nitems,
    const size_t nfactors,
    const size_t * const factors,
    const size_t stride,
    const size_t * const inputs,
    size_t * const outputs
) {
  if (nfactors < 1) {
    *outputs = inputs[0];
    return 0;
  }
  const size_t factor = factors[nfactors - 1];
  for (size_t i = 0; i < factor; i++) {
    rearrange_indices(
        nitems / factor,
        nfactors - 1,
        factors,
        stride * factor,
        inputs + stride * i,
        outputs + i * nitems / factor
    );
  }
  return 0;
}

static int construct_swap_list(
    const size_t nitems,
    const size_t nfactors,
    const size_t * const factors,
    size_t * const nswap_list,
    swap_list_item_t * const swap_list
) {
  size_t * const ordered = memory_alloc(nitems * sizeof(size_t));
  size_t * const arranged = memory_alloc(nitems * sizeof(size_t));
  for (size_t i = 0; i < nitems; i++) {
    ordered[i] = i;
  }
  rearrange_indices(nitems, nfactors, factors, 1, ordered, arranged);
  *nswap_list = 0;
  for (size_t i = 0; i < nitems; i++) {
    if (ordered[i] == arranged[i]) {
      continue;
    }
    for (size_t j = i + 1; j < nitems; j++) {
      if (ordered[j] == arranged[i]) {
        const size_t tmp = ordered[i];
        ordered[i] = ordered[j];
        ordered[j] = tmp;
        swap_list_item_t * const swap_list_item = swap_list + *nswap_list;
        swap_list_item->i = i;
        swap_list_item->j = j;
        *nswap_list += 1;
        break;
      }
    }
  }
  memory_free(ordered);
  memory_free(arranged);
  return 0;
}

static int apply_swap_list(
    const size_t nswap_list,
    const swap_list_item_t * const swap_list,
    double * const values
) {
  for (size_t n = 0; n < nswap_list; n++) {
    const swap_list_item_t * const swap_list_item = swap_list + n;
    const size_t i = swap_list_item->i;
    const size_t j = swap_list_item->j;
    const double tmp = values[i];
    values[i] = values[j];
    values[j] = tmp;
  }
  return 0;
}

static int compute_complex_multiplication(
    const double real_in_0,
    const double imag_in_0,
    const double real_in_1,
    const double imag_in_1,
    double * const real_out,
    double * const imag_out
) {
  *real_out = real_in_0 * real_in_1 - imag_in_0 * imag_in_1;
  *imag_out = real_in_0 * imag_in_1 + imag_in_0 * real_in_1;
  return 0;
}

static int execute_stage(
    const size_t nitems,
    const trig_t * const trigs,
    const size_t factor,
    const size_t stride,
    const double sign,
    double * const restrict reals,
    double * const restrict imags
) {
  const size_t block_size = factor * stride;
  const size_t nblocks = nitems / block_size;
  const size_t factor_quotient = nitems / factor;
  for (size_t block_index = 0; block_index < nblocks; block_index++) {
    const size_t block_head = block_size * block_index;
    for (size_t l = 0; l < stride; l++) {
      // butterfly
      double local_reals[RADIX_MAX];
      double local_imags[RADIX_MAX];
      // load values and apply twiddle factors
      for (size_t n = 0; n < factor; n++) {
        const size_t input_index = block_head + n * stride + l;
        // W_{block_size}^{l * n}
        const size_t twiddle_index = l * n * nblocks;
        const trig_t * const trig = trigs + twiddle_index;
        compute_complex_multiplication(
          reals[input_index], imags[input_index],
          trig->cos, trig->sin * sign,
          local_reals + n, local_imags + n
        );
      }
      // apply twiddle factors and save back values
      for (size_t p = 0; p < factor; p++) {
        const size_t output_index = block_head + p * stride + l;
        double * const output_real = reals + output_index;
        double * const output_imag = imags + output_index;
        *output_real = 0.;
        *output_imag = 0.;
        for (size_t n = 0; n < factor; n++) {
          // W_{factor}^{p * n}
          const size_t twiddle_index = (p * n) % factor * factor_quotient;
          const trig_t * const trig = trigs + twiddle_index;
          double real = 0.;
          double imag = 0.;
          compute_complex_multiplication(
            local_reals[n], local_imags[n],
            trig->cos, trig->sin * sign,
            &real, &imag
          );
          *output_real += real;
          *output_imag += imag;
        }
      }
    }
  }
  return 0;
}

static int execute_inplace_transform(
    const dft_plan_t * const plan,
    const double sign,
    double * const restrict reals,
    double * const restrict imags
) {
  const size_t nitems = plan->nitems;
  const size_t nfactors = plan->nfactors;
  const size_t * const factors = plan->factors;
  const trig_t * const trigs = plan->trigs;
  const size_t nswap_list = plan->nswap_list;
  const swap_list_item_t * const swap_list = plan->swap_list;
  apply_swap_list(nswap_list, swap_list, reals);
  apply_swap_list(nswap_list, swap_list, imags);
  size_t stride = 1;
  for (size_t i = 0; i < nfactors; i++) {
    const size_t factor = factors[i];
    execute_stage(nitems, trigs, factor, stride, sign, reals, imags);
    stride *= factor;
  }
  return 0;
}

static int scale(
    const size_t nitems,
    const double prefactor,
    double * const values
) {
  for (size_t i = 0; i < nitems; i++) {
    values[i] *= prefactor;
  }
  return 0;
}

int dft_execute_inplace_forward_transform(
    const dft_plan_t * const plan,
    double * const restrict reals,
    double * const restrict imags
) {
  execute_inplace_transform(plan, -1., reals, imags);
  return 0;
}

int dft_execute_inplace_backward_transform(
    const dft_plan_t * const plan,
    double * const restrict reals,
    double * const restrict imags
) {
  execute_inplace_transform(plan, 1., reals, imags);
  const size_t nitems = plan->nitems;
  scale(nitems, 1. / nitems, reals);
  scale(nitems, 1. / nitems, imags);
  return 0;
}

int dft_execute_outofplace_forward_transform(
    const dft_plan_t * const plan,
    const double * const restrict input_reals,
    const double * const restrict input_imags,
    double * const restrict output_reals,
    double * const restrict output_imags
) {
  const size_t nitems = plan->nitems;
  for (size_t i = 0; i < nitems; i++) {
    output_reals[i] = input_reals[i];
    output_imags[i] = input_imags[i];
  }
  dft_execute_inplace_forward_transform(plan, output_reals, output_imags);
  return 0;
}

int dft_execute_outofplace_backward_transform(
    const dft_plan_t * const plan,
    const double * const restrict input_reals,
    const double * const restrict input_imags,
    double * const restrict output_reals,
    double * const restrict output_imags
) {
  const size_t nitems = plan->nitems;
  for (size_t i = 0; i < nitems; i++) {
    output_reals[i] = input_reals[i];
    output_imags[i] = input_imags[i];
  }
  dft_execute_inplace_backward_transform(plan, output_reals, output_imags);
  return 0;
}

int dft_init_plan(
    const size_t nitems,
    dft_plan_t ** const plan
) {
  if (nitems < 1) {
    fprintf(stderr, "nitems should be at least 1: %zu\n", nitems);
    *plan = NULL;
    return 1;
  }
  *plan = memory_alloc(1 * sizeof(dft_plan_t));
  (*plan)->trigs = NULL;
  (*plan)->factors = NULL;
  (*plan)->swap_list = NULL;
  (*plan)->nitems = nitems;
  // compute factors
  // NOTE: at most log2(nitems) <= CHAR_BIT * sizeof(size_t) factors exist
  size_t * const nfactors = &(*plan)->nfactors;
  size_t ** const factors = &(*plan)->factors;
  *factors = memory_alloc(CHAR_BIT * sizeof(size_t) * sizeof(size_t));
  if (0 != prime_factorize(nitems, nfactors, *factors)) {
    fprintf(stderr, "failed to prime-factorize: %zu\n", nitems);
    dft_destroy_plan(plan);
    return 1;
  }
  // prepare swap list
  // NOTE: at most nitems entries exist
  size_t * const nswap_list = &(*plan)->nswap_list;
  swap_list_item_t ** const swap_list = &(*plan)->swap_list;
  *swap_list = memory_alloc(nitems * sizeof(swap_list_item_t));
  if (0 != construct_swap_list(
      nitems,
      (*plan)->nfactors,
      (*plan)->factors,
      nswap_list,
      *swap_list
  )) {
    fprintf(stderr, "failed to construct swap list\n");
    dft_destroy_plan(plan);
    return 1;
  }
  // prepare trigonometric table
  trig_t ** const trigs = &(*plan)->trigs;
  *trigs = memory_alloc(nitems * sizeof(trig_t));
  for (size_t k = 0; k < nitems; k++) {
    trig_t * const trig = *trigs + k;
    trig->cos = cos(2. * pi * k / nitems);
    trig->sin = sin(2. * pi * k / nitems);
  }
  return 0;
}

int dft_destroy_plan(
    dft_plan_t ** const plan
) {
  memory_free((*plan)->trigs);
  memory_free((*plan)->factors);
  memory_free((*plan)->swap_list);
  memory_free(*plan);
  *plan = NULL;
  return 0;
}

