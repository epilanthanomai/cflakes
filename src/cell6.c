#include <stdio.h>
#include "cell6.h"
#include "util.h"

/* geometry */

struct c6_geometry *c6_make_geometry(int max_dim) {
  struct c6_geometry *geo = must_malloc(sizeof(struct c6_geometry));
  geo->center = max_dim;
  geo->row_length = 2 * max_dim + 1;
  return geo;
}

void c6_free_geometry(struct c6_geometry *geo) {
  free(geo);
}

/* state */

struct c6_state *c6_make_state(struct c6_geometry *geo) {
  struct c6_state *state = must_malloc(sizeof(struct c6_state));
  state->geo = geo;

  int num_values = geo->row_length * geo->row_length;
  state->cells = must_malloc(sizeof(float) * num_values);

  return state;
}

void c6_free_state(struct c6_state *state) {
  free(state->cells);
  free(state);
}

int c6_state_index(struct c6_state *state, int row, int col) {
  return state->geo->row_length * (row + state->geo->center) + 
    (col + state->geo->center);
}

int c6_state_point_valid(struct c6_state *state, int row, int col) {
  return abs(row + col) <= state->geo->center;
}

void c6_dump_state(struct c6_state *state) {
  C6_GEO_EACH_CELL(state->geo, r, c,
    {}, {
      if (c6_state_point_valid(state, r, c)) {
        int idx = c6_state_index(state, r, c);
        fprintf(stderr, "%6.3f ", state->cells[idx]);
      } else {
        fprintf(stderr, "%6s ", "");
      }
    },{
      fputc('\n', stderr);
    });
}
