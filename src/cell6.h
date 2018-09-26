#pragma once

#include <stdlib.h>

// private interface needed for macros

struct c6_geometry {
  int center;
  int row_length;
};

struct c6_state {
  struct c6_geometry *geo;
  float *cells;
};


// public interface: function

struct c6_geometry *c6_make_geometry(int max_dim);
void c6_free_geometry(struct c6_geometry*);

struct c6_state *c6_make_state(struct c6_geometry*);
void c6_free_state(struct c6_state*);
int c6_state_index(struct c6_state *, int row, int col);
int c6_state_point_valid(struct c6_state*, int row, int col);

void c6_dump_state(struct c6_state*);

// public interface: inlines and macros

#define C6_STATE_EACH_CELL(state, row_v, col_v, cell_block, row_block)  \
{  \
  for (int row_v=-(state)->geo->center; row_v <= (state)->geo->center; row_v++) {  \
    for (int col_v=-(state)->geo->center; col_v <= (state)->geo->center; col_v++) {  \
      cell_block;  \
    }  \
    { row_block; }  \
  }  \
}
