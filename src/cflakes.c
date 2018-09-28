#include <stdio.h>
#include "cell6.h"
#include "path.h"
#include "reiter.h"
#include "svg.h"

#define MAX_DIM 200
#define MAX_ITERS 10000

#define BG_LEVEL 0.4
#define CONST_GAIN 0.001
#define NDIFF_WEIGHT 1.0
#define SVG_DIM "8in"

int main(int argc, char **argv) {
  struct c6_state_geometry *geo = c6_make_geometry(MAX_DIM);
  struct c6_state *state = c6_make_state(geo);

  rsf_init_state(state, BG_LEVEL);
  state = rsf_state_advance_to_edge(
      state, MAX_ITERS, CONST_GAIN, NDIFF_WEIGHT);

  struct pa_path_set *ps = rsf_make_traced_path_set(state);
  svg_print_paths(ps, MAX_DIM, SVG_DIM);

  pa_free_path_set(ps);
  c6_free_state(state);
  c6_free_geometry(geo);
  return 0;
}
