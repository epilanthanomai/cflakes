#include "cell6.h"
#include "reiter.h"

#define MAX_DIM 5
#define BG_LEVEL 0.4

int main(int argc, char **argv) {
  struct c6_geometry *geo = c6_make_geometry(MAX_DIM);
  struct c6_state *state = c6_make_state(geo);
  rsf_init_state(state, BG_LEVEL);

  c6_dump_state(state);

  c6_free_state(state);
  c6_free_geometry(geo);
  return 0;
}
