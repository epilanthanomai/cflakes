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

#define C6_GEO_EACH_CELL(geo, row_v, col_v, before_row, cell, after_row)  \
{  \
  for (int row_v=-(geo)->center; row_v <= (geo)->center; row_v++) {  \
    { before_row; }  \
    for (int col_v=-(geo)->center; col_v <= (geo)->center; col_v++) {  \
      cell;  \
    }  \
    { after_row; }  \
  }  \
}
