#include <stdio.h>
#include <stdlib.h>
#include "cell6.h"
#include "util.h"

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
    geo->rows[row].start_index = index;
    index += row_length;
    geo->rows[row].end_index = index;
  }

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
      for (int i = r; i < state->geo->center; i++) {
        fprintf(stderr, "%6s ", "");
      }
    }, {
      fprintf(stderr, "%6.3f ", state->cells[i]);
    },{
      fputc('\n', stderr);
    });
}
