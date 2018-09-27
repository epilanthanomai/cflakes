#pragma once

// private interface needed for macros

struct c6_row_geometry {
  int start_index;
  int end_index;
};

struct c6_state_geometry {
  int center;
  int max_row;
  struct c6_row_geometry *rows;
};

struct c6_state {
  struct c6_state_geometry *geo;
  float *cells;
};


// public interface: function

struct c6_state_geometry *c6_make_geometry(int max_dim);
void c6_free_geometry(struct c6_state_geometry*);
int c6_geo_index(struct c6_state_geometry *, int row, int col);

struct c6_state *c6_make_state(struct c6_state_geometry*);
void c6_free_state(struct c6_state*);
void c6_dump_state(struct c6_state*);

// public interface: inlines and macros

#define C6_GEO_EACH_CELL(geo, idx_v, cell)  \
{  \
  int __max_idx = geo->rows[geo->max_row-1].end_index;  \
  for (int idx_v=0; idx_v < __max_idx; idx_v++) { cell }  \
}

#define C6_GEO_EACH_ROW_CELL(geo, row_v, col_v, idx_v, before_row, cell, after_row)  \
{  \
  int row_v = 0;  \
  int col_v = 0;  \
  int idx_v = 0;  \
  int __max_idx = geo->rows[geo->max_row-1].end_index;  \
  while (idx_v < __max_idx) {  \
    if (idx_v == geo->rows[row_v].end_index) {  \
      row_v++;  \
      col_v = 0;  \
    }  \
    if (col_v == 0) { before_row; }  \
    { cell }  \
    idx_v++;  \
    col_v++;  \
    if (idx_v == geo->rows[row_v].end_index) {  \
      { after_row }  \
    }  \
  }  \
}
