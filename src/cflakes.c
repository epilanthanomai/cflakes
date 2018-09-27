#include <stdio.h>
#include "cell6.h"
#include "path.h"
#include "reiter.h"

#define MAX_DIM 20
#define BG_LEVEL 0.4
#define MAX_ITERS 2000

int main(int argc, char **argv) {
  struct c6_state_geometry *geo = c6_make_geometry(MAX_DIM);
  struct c6_state *state = c6_make_state(geo);

  rsf_init_state(state, BG_LEVEL);
  state = rsf_state_advance_to_edge(state, MAX_ITERS);
  rsf_state_dump_ice(state);

  struct pa_path_set *ps = rsf_make_traced_path_set(state);

  pa_free_path_set(ps);
  c6_free_state(state);
  c6_free_geometry(geo);
  return 0;
}
