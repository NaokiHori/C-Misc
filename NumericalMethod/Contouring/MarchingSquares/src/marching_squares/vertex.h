#if !defined(VERTEX_H)
#define VERTEX_H

// simply saying that one cell (square) has four vertices
#define NVERTICES 4

typedef enum {
  VERTEX_LEFT_BOTTOM  = 0,
  VERTEX_RIGHT_BOTTOM = 1,
  VERTEX_RIGHT_TOP    = 2,
  VERTEX_LEFT_TOP     = 3,
} vertex_index_t;

#endif // VERTEX_H
