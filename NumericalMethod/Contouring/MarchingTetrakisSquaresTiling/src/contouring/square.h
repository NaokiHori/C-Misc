#if !defined(SQUARE_H)
#define SQUARE_H

#define NUM_SQUARE_EDGES 4
#define NUM_SQUARE_VERTICES 4

typedef enum {
  SQUARE_EDGE_YNEG = 0,
  SQUARE_EDGE_XPOS = 1,
  SQUARE_EDGE_YPOS = 2,
  SQUARE_EDGE_XNEG = 3,
} square_edge_index_t;

typedef enum {
  SQUARE_VERTEX_LEFT_BOTTOM  = 0,
  SQUARE_VERTEX_RIGHT_BOTTOM = 1,
  SQUARE_VERTEX_RIGHT_TOP    = 2,
  SQUARE_VERTEX_LEFT_TOP     = 3,
} square_vertex_index_t;

extern double get_centered_value (
    const double vertex_values[NUM_SQUARE_VERTICES]
);

#endif // SQUARE_H
