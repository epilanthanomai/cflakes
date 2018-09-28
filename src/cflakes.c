#include <stdlib.h>
#include <stdio.h>
#include "cell6.h"
#include "path.h"
#include "reiter.h"
#include "svg.h"

#define MAX_DIM 200
#define MAX_ITERS 10000

#define BG_BASE 0.3
#define BG_VAR 0.5
#define GAIN_BASE 0.0
#define GAIN_VAR 0.005
#define NDW_BASE 0.5
#define NDW_VAR 2.5
#define DIM_BASE 2.0
#define DIM_VAR 1.0

static float rand_float(float base, float var) {
  return base + (float) rand() * var / (float) RAND_MAX;
}

int main(int argc, char **argv) {
  struct c6_state_geometry *geo = c6_make_geometry(MAX_DIM);
  struct c6_state *state = c6_make_state(geo);

  float bg_level = rand_float(BG_BASE, BG_VAR);
  float const_gain = rand_float(GAIN_BASE, GAIN_VAR);
  float ndiff_weight = rand_float(NDW_BASE, NDW_VAR);
  float svg_dim = rand_float(DIM_BASE, DIM_VAR);

  char svg_dim_s[12] = {};;
  snprintf(svg_dim_s, sizeof(svg_dim_s)-1, "%8.6fin", svg_dim);


  rsf_init_state(state, bg_level);
  state = rsf_state_advance_to_edge(
      state, MAX_ITERS, const_gain, ndiff_weight);

  struct pa_path_set *ps = rsf_make_traced_path_set(state);
  svg_print_paths(ps, MAX_DIM, svg_dim_s);

  pa_free_path_set(ps);
  c6_free_state(state);
  c6_free_geometry(geo);
  return 0;
}
