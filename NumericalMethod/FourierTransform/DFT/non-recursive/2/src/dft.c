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

static const size_t my_stack_node_dummy_value = SIZE_MAX;

#define MY_STACK_NODE_NITEMS 2

typedef struct my_stack_node_t {
  // NOTE: only the first item is used by prime factorization
  size_t items[MY_STACK_NODE_NITEMS];
  struct my_stack_node_t * next;
} my_stack_node_t;

typedef struct {
  size_t nitems;
  my_stack_node_t * head;
} my_stack_t;

static int push_node(
    my_stack_t * const stack,
    const size_t items[MY_STACK_NODE_NITEMS]
) {
  my_stack_node_t * const node = malloc(sizeof(my_stack_node_t));
  for (size_t i = 0; i < MY_STACK_NODE_NITEMS; i++) {
    node->items[i] = items[i];
  }
  node->next = stack->head;
  stack->head = node;
  stack->nitems += 1;
  return 0;
}

static int pop_node(
    my_stack_t * const stack,
    size_t (* const items)[MY_STACK_NODE_NITEMS]
) {
  if (NULL == stack->head || 0 == stack->nitems) {
    return 1;
  }
  my_stack_node_t * const node = stack->head;
  for (size_t i = 0; i < MY_STACK_NODE_NITEMS; i++) {
    (*items)[i] = node->items[i];
  }
  stack->head = node->next;
  stack->nitems -= 1;
  memory_free(node);
  return 0;
}

static int prime_factorize(
    const size_t nitems,
    size_t * const nfactors,
    size_t ** const factors
) {
  // push factors to a stack
  my_stack_t stack = {0};
  size_t n = nitems;
  size_t d = 2;
  while (d * d <= n) {
    while (0 == n % d) {
      push_node(
          &stack,
          (size_t [MY_STACK_NODE_NITEMS]){d, my_stack_node_dummy_value}
      );
      n /= d;
    }
    d += 1;
  }
  if (1 < n) {
    push_node(
        &stack,
        (size_t [MY_STACK_NODE_NITEMS]){n, my_stack_node_dummy_value}
    );
  }
  *nfactors = stack.nitems;
  *factors = memory_alloc(*nfactors * sizeof(size_t));
  for (size_t i = 0; i < *nfactors; i++) {
    size_t buffer[MY_STACK_NODE_NITEMS] = {
      my_stack_node_dummy_value,
      my_stack_node_dummy_value,
    };
    pop_node(&stack, &buffer);
    // sort by ascending order
    size_t * const factor = *factors + *nfactors - i - 1;
    *factor = buffer[0];
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
    swap_list_item_t ** const swap_list
) {
  size_t * const ordered = memory_alloc(nitems * sizeof(size_t));
  size_t * const arranged = memory_alloc(nitems * sizeof(size_t));
  for (size_t i = 0; i < nitems; i++) {
    ordered[i] = i;
  }
  rearrange_indices(nitems, nfactors, factors, 1, ordered, arranged);
  my_stack_t stack = {0};
  for (size_t i = 0; i < nitems; i++) {
    if (ordered[i] == arranged[i]) {
      continue;
    }
    for (size_t j = i + 1; j < nitems; j++) {
      if (ordered[j] == arranged[i]) {
        const size_t tmp = ordered[i];
        ordered[i] = ordered[j];
        ordered[j] = tmp;
        push_node(&stack, (size_t [MY_STACK_NODE_NITEMS]){i, j});
      }
    }
  }
  *nswap_list = stack.nitems;
  *swap_list = memory_alloc(*nswap_list * sizeof(swap_list_item_t));
  for (size_t n = 0; n < *nswap_list; n++) {
    size_t buffer[MY_STACK_NODE_NITEMS] = {
      my_stack_node_dummy_value,
      my_stack_node_dummy_value,
    };
    pop_node(&stack, &buffer);
    swap_list_item_t * const swap_list_item = *swap_list + *nswap_list - n - 1;
    swap_list_item->i = buffer[0];
    swap_list_item->j = buffer[1];
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

static int zero_clear_buffer(
    const size_t nitems,
    double * const values
) {
  for (size_t i = 0; i < nitems; i++) {
    values[i] = 0.;
  }
  return 0;
}

static int compute_complex_multiplication_and_add(
    const double real_in_0,
    const double imag_in_0,
    const double real_in_1,
    const double imag_in_1,
    double * const real_out,
    double * const imag_out
) {
  *real_out += real_in_0 * real_in_1 - imag_in_0 * imag_in_1;
  *imag_out += real_in_0 * imag_in_1 + imag_in_0 * real_in_1;
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
      zero_clear_buffer(factor, local_reals);
      zero_clear_buffer(factor, local_imags);
      // load values and apply twiddle factors
      for (size_t n = 0; n < factor; n++) {
        const size_t input_index = block_head + n * stride + l;
        // W_{block_size}^{l * n}
        const size_t twiddle_index = (l * n) % block_size * nblocks;
        const trig_t * const trig = trigs + twiddle_index;
        compute_complex_multiplication_and_add(
          reals[input_index], imags[input_index],
          trig->cos, trig->sin * sign,
          local_reals + n, local_imags + n
        );
      }
      // apply twiddle factors and save back values
      for (size_t p = 0; p < factor; p++) {
        double real = 0.;
        double imag = 0.;
        for (size_t n = 0; n < factor; n++) {
          // W_{factor}^{p * n}
          const size_t twiddle_index = (p * n) % factor * factor_quotient;
          const trig_t * const trig = trigs + twiddle_index;
          compute_complex_multiplication_and_add(
            local_reals[n], local_imags[n],
            trig->cos, trig->sin * sign,
            &real, &imag
          );
        }
        const size_t output_index = block_head + p * stride + l;
        reals[output_index] = real;
        imags[output_index] = imag;
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

int dft_init_plan(
    const size_t nitems,
    dft_plan_t ** const plan
) {
  if (nitems < 1) {
    fprintf(stderr, "nitems should be at least 1: %zu\n", nitems);
    return 1;
  }
  *plan = memory_alloc(1 * sizeof(dft_plan_t));
  (*plan)->nitems = nitems;
  trig_t ** const trigs = &(*plan)->trigs;
  *trigs = memory_alloc(nitems * sizeof(trig_t));
  // prepare cosine / sine tables
  for (size_t k = 0; k < nitems; k++) {
    trig_t * const trig = *trigs + k;
    trig->cos = cos(2. * pi * k / nitems);
    trig->sin = sin(2. * pi * k / nitems);
  }
  if (0 != prime_factorize(nitems, &(*plan)->nfactors, &(*plan)->factors)) {
    fprintf(stderr, "failed to prime-factorize: %zu\n", nitems);
    dft_destroy_plan(plan);
    return 1;
  }
  for (size_t i = 0; i < (*plan)->nfactors; i++) {
    const size_t factor = (*plan)->factors[i];
    if (RADIX_MAX < factor) {
      fprintf(stderr, "prime number is not supported: %zu\n", factor);
      dft_destroy_plan(plan);
      return 1;
    }
  }
  if (0 != construct_swap_list(
      nitems,
      (*plan)->nfactors,
      (*plan)->factors,
      &(*plan)->nswap_list,
      &(*plan)->swap_list)
  ) {
    fprintf(stderr, "failed to construct swap list\n");
    dft_destroy_plan(plan);
    return 1;
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

