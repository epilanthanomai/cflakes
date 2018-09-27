#include <stdio.h>
#include <stdlib.h>
#include "cell6.h"
#include "util.h"


// row, col:
//
//         -1, 0
//   0,-1          -1, +1
//          0, 0
//  +1,-1           0, +1
//         +1, 0

static struct {
  int row_d;
  int col_d;
} neighbor_candidates[MAX_NEIGHBORS] = {
  { -1,  0 },
  { -1, +1 },
  {  0, -1 },
  {  0, +1 },
  { +1, -1 },
  { +1,  0 }
};

/* geometry */

struct c6_state_geometry *c6_make_geometry(int max_dim) {
  int max_row = 2 * max_dim + 1;

  struct c6_state_geometry *geo = must_malloc(sizeof(struct c6_state_geometry));
  geo->center = max_dim;
  geo->max_row = max_row;

  geo->rows = must_malloc(max_row * sizeof(struct c6_row_geometry));
  int index = 0;
  for (int row = 0; row < max_row; row++) {
    int row_length = max_row - abs(max_dim - row);
    geo->rows[row].col_offset = (row < max_dim) ? (max_dim - row) : 0;
    geo->rows[row].start_index = index;
    index += row_length;
    geo->rows[row].end_index = index;
  }

  geo->cells = must_malloc(index * sizeof(struct c6_cell_geometry));
  C6_GEO_EACH_ROW_CELL(geo, r, c, i, {
    }, {
      geo->cells[i].num_neighbors = 0;
      for (int nc=0; nc < MAX_NEIGHBORS; nc++) {
        int nc_r = r + neighbor_candidates[nc].row_d;
        int nc_c = c + neighbor_candidates[nc].col_d;
        if ((nc_r >= 0) && (nc_r < max_row)) {
          int col_offset = geo->rows[r].col_offset - geo->rows[nc_r].col_offset;
          int nc_i = geo->rows[nc_r].start_index + nc_c + col_offset;
          if ((nc_i >= geo->rows[nc_r].start_index) &&
              (nc_i < geo->rows[nc_r].end_index)) {
            int n = geo->cells[i].num_neighbors++;
            geo->cells[i].neighbor_indexes[n] = nc_i;
          }
        }
      }
    }, {});

  geo->num_edges = max_dim * 6;
  geo->edges = must_malloc(geo->num_edges * sizeof(int));
  int num_edges = 0;
  C6_GEO_EACH_CELL(geo, i, {
      if (geo->cells[i].num_neighbors < MAX_NEIGHBORS) {
        geo->edges[num_edges++] = i;
      }
    });

  return geo;
}

void c6_free_geometry(struct c6_state_geometry *geo) {
  free(geo);
}

int c6_geo_index(struct c6_state_geometry *geo, int row, int col) {
  int abs_row = geo->center + row;
  int abs_col = geo->center + col;
  return geo->rows[abs_row].start_index + abs_col;
}

/* state */

struct c6_state *c6_make_state(struct c6_state_geometry *geo) {
  struct c6_state *state = must_malloc(sizeof(struct c6_state));
  state->geo = geo;

  int num_values = geo->rows[geo->max_row-1].end_index;
  state->cells = must_malloc(sizeof(float) * num_values);

  return state;
}

void c6_free_state(struct c6_state *state) {
  free(state->cells);
  free(state);
}

void c6_dump_state(struct c6_state *state) {
  C6_GEO_EACH_ROW_CELL(state->geo, r, c, i,
    {
      for (int j = r; j < state->geo->center; j++) {
        fprintf(stderr, "%6s ", "");
      }
    }, {
      fprintf(stderr, "%6.3f ", state->cells[i]);
    },{
      fputc('\n', stderr);
    });
}
