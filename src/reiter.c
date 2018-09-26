#include <stdio.h>
#include <stdlib.h>
#include "reiter.h"
#include "util.h"

/* geometry */

struct rsf_geometry {
  int center;
  int row_length;
};

struct rsf_geometry *rsf_make_geometry(int max_dim) {
  struct rsf_geometry *geo = must_malloc(sizeof(struct rsf_geometry));
  geo->center = max_dim;
  geo->row_length = 2 * max_dim + 1;
  return geo;
}

void rsf_free_geometry(struct rsf_geometry *geo) {
  free(geo);
}

/* state */

struct rsf_state {
  struct rsf_geometry *geo;
  float *cells;
};

static inline int rsf_state_index(struct rsf_state *state, int row, int col) {
  return state->geo->row_length * (row + state->geo->center) + 
    (col + state->geo->center);
}

static inline int rsf_state_center(struct rsf_state *state) {
  return rsf_state_index(state, 0, 0);
}

static inline int rsf_state_point_valid(struct rsf_state *state, int row, int col) {
  return abs(row + col) <= state->geo->center;
}

struct rsf_state *rsf_make_state(struct rsf_geometry *geo, float bg_level) {
  struct rsf_state *state = must_malloc(sizeof(struct rsf_state));
  state->geo = geo;

  int num_values = geo->row_length * geo->row_length;
  state->cells = must_malloc(sizeof(float) * num_values);
  for (int i=0; i < num_values; i++)
    state->cells[i] = bg_level;
  state->cells[rsf_state_center(state)] = 1.0;

  return state;
}

void rsf_free_state(struct rsf_state *state) {
  free(state->cells);
  free(state);
}

void rsf_dump_state(struct rsf_state *state) {
  for (int r=-state->geo->center; r <= state->geo->center; r++) {
    for (int c=-state->geo->center; c <= state->geo->center; c++) {
      if (rsf_state_point_valid(state, r, c)) {
        int idx = rsf_state_index(state, r, c);
        fprintf(stderr, "%6.3f ", state->cells[idx]);
      } else {
        fprintf(stderr, "%6s ", "");
      }
    }
    fputc('\n', stderr);
  }
}
