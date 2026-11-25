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

struct dft_plan_t {
  // degrees of freedom
  size_t nitems;
  size_t nfactors;
  size_t * factors;
  // pre-computed trigonometric values
  trig_t * trigs;
  // reorder input buffer
  size_t nswap_list;
  size_t (* swap_list)[2];
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

static const size_t singly_linked_node_dummy_value = SIZE_MAX;

typedef struct singly_linked_node_t {
  // NOTE: only the first item is used by prime factorization
  size_t items[2];
  struct singly_linked_node_t * next;
} singly_linked_node_t;

typedef struct {
  size_t nitems;
  singly_linked_node_t * root;
} singly_linked_list_t;

static int add_node(
    singly_linked_list_t * const list,
    const size_t items[2]
) {
  singly_linked_node_t * const node = malloc(sizeof(singly_linked_node_t));
  node->items[0] = items[0];
  node->items[1] = items[1];
  node->next = list->root;
  list->root = node;
  list->nitems += 1;
  return 0;
}

static int pop_node(
    singly_linked_list_t * const list,
    size_t (* const items)[2]
) {
  if (NULL == list->root || 0 == list->nitems) {
    return 1;
  }
  singly_linked_node_t * const node = list->root;
  (*items)[0] = node->items[0];
  (*items)[1] = node->items[1];
  list->root = node->next;
  list->nitems -= 1;
  memory_free(node);
  return 0;
}

static int prime_factorize(
    const size_t nitems,
    size_t * const nfactors,
    size_t ** const factors
) {
  // add factors to a singly-linked list
  singly_linked_list_t list = {0};
  size_t n = nitems;
  size_t d = 2;
  while (d * d <= n) {
    while (0 == n % d) {
      add_node(&list, (size_t [2]){d, singly_linked_node_dummy_value});
      n /= d;
    }
    d += 1;
  }
  if (1 < n) {
    add_node(&list, (size_t [2]){n, singly_linked_node_dummy_value});
  }
  *nfactors = list.nitems;
  *factors = memory_alloc(*nfactors * sizeof(size_t));
  for (size_t i = 0; i < *nfactors; i++) {
    size_t buffer[2] = {
      singly_linked_node_dummy_value,
      singly_linked_node_dummy_value,
    };
    pop_node(&list, &buffer);
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
    size_t (** const swap_list)[2]
) {
  size_t * const ordered = memory_alloc(nitems * sizeof(size_t));
  size_t * const arranged = memory_alloc(nitems * sizeof(size_t));
  for (size_t i = 0; i < nitems; i++) {
    ordered[i] = i;
  }
  rearrange_indices(nitems, nfactors, factors, 1, ordered, arranged);
  singly_linked_list_t list = {0};
  for (size_t i = 0; i < nitems; i++) {
    if (ordered[i] == arranged[i]) {
      continue;
    }
    for (size_t j = i + 1; j < nitems; j++) {
      if (ordered[j] == arranged[i]) {
        const size_t tmp = ordered[i];
        ordered[i] = ordered[j];
        ordered[j] = tmp;
        add_node(&list, (size_t [2]){i, j});
      }
    }
  }
  *nswap_list = list.nitems;
  *swap_list = memory_alloc(*nswap_list * sizeof(size_t [2]));
  for (size_t i = 0; i < *nswap_list; i++) {
    size_t buffer[2] = {
      singly_linked_node_dummy_value,
      singly_linked_node_dummy_value,
    };
    pop_node(&list, &buffer);
    (*swap_list)[*nswap_list - i - 1][0] = buffer[0];
    (*swap_list)[*nswap_list - i - 1][1] = buffer[1];
  }
  memory_free(ordered);
  memory_free(arranged);
  return 0;
}

static int apply_swap_list(
    const size_t nswap_list,
    size_t (* const swap_list)[2],
    double * const values
) {
  for (size_t i = 0; i < nswap_list; i++) {
    size_t (* const indices)[2] = swap_list + i;
    const double tmp = values[(*indices)[0]];
    values[(*indices)[0]] = values[(*indices)[1]];
    values[(*indices)[1]] = tmp;
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
  for (size_t block_index = 0; block_index < nblocks; block_index++) {
    const size_t block_head = block_size * block_index;
    for (size_t k = 0; k < stride; k++) {
      // butterfly
      double rs[RADIX_MAX] = {0.};
      double is[RADIX_MAX] = {0.};
      for (size_t l = 0; l < factor; l++) {
        const size_t m = block_head + k + l * stride;
        // W_{block_size}^{k * l}
        const trig_t * const trig = trigs + ((k * l) % block_size) * (nitems / block_size);
        compute_complex_multiplication_and_add(
          reals[m], imags[m],
          trig->cos, trig->sin * sign,
          rs + l, is + l
        );
      }
      for (size_t l = 0; l < factor; l++) {
        double r = 0.;
        double i = 0.;
        for (size_t m = 0; m < factor; m++) {
          // W_{factor}^{l * m}
          const trig_t * const trig = trigs + ((l * m) % factor) * (nitems / factor);
          compute_complex_multiplication_and_add(
            rs[m], is[m],
            trig->cos, trig->sin * sign,
            &r, &i
          );
        }
        const size_t m = block_head + k + l * stride;
        reals[m] = r;
        imags[m] = i;
      }
    }
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
  size_t (* const swap_list)[2] = plan->swap_list;
  apply_swap_list(nswap_list, swap_list, reals);
  apply_swap_list(nswap_list, swap_list, imags);
  for (size_t i = 0, stride = 1; i < nfactors; i++) {
    const size_t factor = factors[i];
    execute_stage(nitems, trigs, factor, stride, sign, reals, imags);
    stride *= factor;
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

