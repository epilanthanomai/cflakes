#pragma once

/* Copyright 2018, Hope Ranker
 *
 * This file is part of cflakes.
 *
 * cflakes is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * cflakes is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with cflakes. If not, see <https://www.gnu.org/licenses/>.
 */

// private interface needed for macros

#define MAX_NEIGHBORS 6

struct c6_row_geometry {
  int col_offset;
  int start_index;
  int end_index;
};

struct c6_cell_neighbor {
  int index;
  int neighbor_num;
};

struct c6_cell_geometry {
  int num_neighbors;
  struct c6_cell_neighbor neighbors[MAX_NEIGHBORS];
};

struct c6_state_geometry {
  int center;
  int max_row;
  struct c6_row_geometry *rows;
  struct c6_cell_geometry *cells;
  int num_edges;
  int *edges;
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

void c6_dump_neighbor_counts(struct c6_state_geometry*);
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
