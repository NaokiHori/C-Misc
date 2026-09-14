#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "parameter.h"
#include "./modified_cholesky.h"
#include "./setup.h"

static int compute_element_stiffness_matrix(
    const element_t * const element,
    double * const element_stiffness_matrix
) {
  const node_t * const (* const nodes)[N_ELEMENT_NODES] = &element->nodes;
  double vector[NDIMS] = {0.};
  for (size_t dim = 0; dim < NDIMS; dim++) {
    vector[dim] = (*nodes)[1]->position[dim] - (*nodes)[0]->position[dim];
  }
  const double length = element->length;
#if NDIMS == 2
  const double c = vector[0] / length;
  const double s = vector[1] / length;
  element_stiffness_matrix[ 0] = + c * c;
  element_stiffness_matrix[ 1] = + c * s;
  element_stiffness_matrix[ 2] = - c * c;
  element_stiffness_matrix[ 3] = - c * s;
  element_stiffness_matrix[ 4] = + c * s;
  element_stiffness_matrix[ 5] = + s * s;
  element_stiffness_matrix[ 6] = - c * s;
  element_stiffness_matrix[ 7] = - s * s;
  element_stiffness_matrix[ 8] = - c * c;
  element_stiffness_matrix[ 9] = - c * s;
  element_stiffness_matrix[10] = + c * c;
  element_stiffness_matrix[11] = + c * s;
  element_stiffness_matrix[12] = - c * s;
  element_stiffness_matrix[13] = - s * s;
  element_stiffness_matrix[14] = + c * s;
  element_stiffness_matrix[15] = + s * s;
#elif NDIMS == 3
  const double l = vector[0] / length;
  const double m = vector[1] / length;
  const double n = vector[2] / length;
  element_stiffness_matrix[ 0] = + l * l;
  element_stiffness_matrix[ 1] = + l * m;
  element_stiffness_matrix[ 2] = + l * n;
  element_stiffness_matrix[ 3] = - l * l;
  element_stiffness_matrix[ 4] = - l * m;
  element_stiffness_matrix[ 5] = - l * n;
  element_stiffness_matrix[ 6] = + l * m;
  element_stiffness_matrix[ 7] = + m * m;
  element_stiffness_matrix[ 8] = + m * n;
  element_stiffness_matrix[ 9] = - l * m;
  element_stiffness_matrix[10] = - m * m;
  element_stiffness_matrix[11] = - m * n;
  element_stiffness_matrix[12] = + l * n;
  element_stiffness_matrix[13] = + m * n;
  element_stiffness_matrix[14] = + n * n;
  element_stiffness_matrix[15] = - l * n;
  element_stiffness_matrix[16] = - m * n;
  element_stiffness_matrix[17] = - n * n;
  element_stiffness_matrix[18] = - l * l;
  element_stiffness_matrix[19] = - l * m;
  element_stiffness_matrix[20] = - l * n;
  element_stiffness_matrix[21] = + l * l;
  element_stiffness_matrix[22] = + l * m;
  element_stiffness_matrix[23] = + l * n;
  element_stiffness_matrix[24] = - l * m;
  element_stiffness_matrix[25] = - m * m;
  element_stiffness_matrix[26] = - m * n;
  element_stiffness_matrix[27] = + l * m;
  element_stiffness_matrix[28] = + m * m;
  element_stiffness_matrix[29] = + m * n;
  element_stiffness_matrix[30] = - l * n;
  element_stiffness_matrix[31] = - m * n;
  element_stiffness_matrix[32] = - n * n;
  element_stiffness_matrix[33] = + l * n;
  element_stiffness_matrix[34] = + m * n;
  element_stiffness_matrix[35] = + n * n;
#else
#error "unsupported NDIMS"
#endif
  for (size_t n = 0; n < (N_ELEMENT_NODES * NDIMS) * (N_ELEMENT_NODES * NDIMS); n++) {
    element_stiffness_matrix[n] *= YOUNGS_MODULUS * ELEMENT_AREA;
  }
  return 0;
}

static int fix_displacement(
    const size_t n_nodes,
    const size_t dim,
    const size_t index,
    double * const global_stiffness_matrix,
    double * const values
) {
  // zero displacement
  const double fixed_value = 0.;
  const size_t matrix_size = n_nodes * NDIMS;
  const size_t target = index * NDIMS + dim;
  for (size_t n = 0; n < matrix_size; n++) {
    global_stiffness_matrix[target * matrix_size + n] = 0.;
    global_stiffness_matrix[n * matrix_size + target] = 0.;
  }
  global_stiffness_matrix[target * (n_nodes * NDIMS) + target] = 1.;
  values[target] = fixed_value;
  return 0;
}

static int output_result(
    const size_t n_elements,
    const element_t * const elements,
    const double * const node_displacements
) {
  FILE * const fp = fopen("positions.dat", "w");
  if (NULL == fp) {
    return 1;
  }
  double strain_extremum = 0.;
  double * const strains = malloc(n_elements * sizeof(double));
  for (size_t n = 0; n < n_elements; n++) {
    const element_t * const element = elements + n;
    const double length = element->length;
    const node_t * const (* const nodes)[N_ELEMENT_NODES] = &element->nodes;
    double * const strain = strains + n;
    *strain = 0.;
    for (size_t dim = 0; dim < NDIMS; dim++) {
      const double positions[2] = {
        (*nodes)[0]->position[dim] + node_displacements[(*nodes)[0]->index * NDIMS + dim],
        (*nodes)[1]->position[dim] + node_displacements[(*nodes)[1]->index * NDIMS + dim],
      };
      *strain += pow(positions[1] - positions[0], 2.);
    }
    *strain = (sqrt(*strain) - length) / length;
    strain_extremum = fmax(strain_extremum, fabs(*strain));
  }
  for (size_t n = 0; n < n_elements; n++) {
    const element_t * const element = elements + n;
    const node_t * const (* const nodes)[N_ELEMENT_NODES] = &element->nodes;
    const double strain = strains[n];
    for (size_t m = 0; m < N_ELEMENT_NODES; m++) {
      const node_t * const node = (*nodes)[m];
      for (size_t dim = 0; dim < NDIMS; dim++) {
        fprintf(
            fp,
            "% .15e ",
            node->position[dim]
        );
      }
      for (size_t dim = 0; dim < NDIMS; dim++) {
        fprintf(
            fp,
            "% .15e ",
            node_displacements[node->index * NDIMS + dim]
        );
      }
      fprintf(
          fp,
          "0x000000 0x%02X00%02X%s",
          strain < 0. ? (int)(255. * (- strain) / strain_extremum) : 0,
          0. < strain ? (int)(255. * (+ strain) / strain_extremum) : 0,
          N_ELEMENT_NODES - 1 == m ? "\n\n\n" : "\n"
      );
    }
  }
  free(strains);
  fclose(fp);
  return 0;
}

int main(
    void
) {
  size_t n_nodes = 0;
  node_t * nodes = NULL;
  size_t n_elements = 0;
  element_t * elements = NULL;
  // both right-hand-side term (forces) and answer (displacements)
  double * values = NULL;
  setup(&n_nodes, &nodes, &n_elements, &elements, &values);
  // construct stiffness matrix
  double * const global_stiffness_matrix = malloc((n_nodes * NDIMS) * (n_nodes * NDIMS) * sizeof(double));
  for (size_t i = 0; i < n_nodes * NDIMS; i++) {
    for (size_t j = 0; j < n_nodes * NDIMS; j++) {
      global_stiffness_matrix[i * (n_nodes * NDIMS) + j] = 0.;
    }
  }
  for (size_t n = 0; n < n_elements; n++) {
    const element_t * const element = elements + n;
    double element_stiffness_matrix[(N_ELEMENT_NODES * NDIMS) * (N_ELEMENT_NODES * NDIMS)] = {0.};
    compute_element_stiffness_matrix(element, element_stiffness_matrix);
    // embed element stiffness matrix to global stiffness matrix
    const node_t * const (* const local_nodes)[N_ELEMENT_NODES] = &element->nodes;
    for (size_t i = 0; i < N_ELEMENT_NODES; i++) {
      const size_t index_i = (*local_nodes)[i]->index;
      for (size_t j = 0; j < N_ELEMENT_NODES; j++) {
        const size_t index_j = (*local_nodes)[j]->index;
        for (size_t k = 0; k < NDIMS; k++) {
          for (size_t l = 0; l < NDIMS; l++) {
            global_stiffness_matrix[(index_i * NDIMS + k) * (n_nodes * NDIMS) + (index_j * NDIMS + l)]
              += element_stiffness_matrix[(i * NDIMS + k) * (N_ELEMENT_NODES * NDIMS) + (j * NDIMS + l)];
          }
        }
      }
    }
  }
  // impose constraints
  for (size_t n = 0; n < n_nodes; n++) {
    const node_t * const node = nodes + n;
    const size_t index = node->index;
    const bool (* const is_fixed)[NDIMS] = &node->is_fixed;
    for (size_t dim = 0; dim < NDIMS; dim++) {
      if ((*is_fixed)[dim]) {
        fix_displacement(n_nodes, dim, index, global_stiffness_matrix, values);
      }
    }
  }
  // solve system
  modified_cholesky(n_nodes * NDIMS, global_stiffness_matrix, values);
  // check result
  output_result(n_elements, elements, values);
  // clean-up
  free(nodes);
  free(elements);
  free(global_stiffness_matrix);
  free(values);
  return 0;
}

