#include "reiter.h"

#define MAX_DIM 5
#define BG_LEVEL 0.4

int main(int argc, char **argv) {
  struct rsf_geometry *geo = rsf_make_geometry(MAX_DIM);
  struct rsf_state *state = rsf_make_state(geo, BG_LEVEL);

  rsf_dump_state(state);

  rsf_free_state(state);
  rsf_free_geometry(geo);
  return 0;
}
