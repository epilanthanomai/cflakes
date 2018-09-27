#include "cell6.h"

void rsf_init_state(struct c6_state *state, float bg_level) {
  C6_GEO_EACH_CELL(state->geo, i, {
      state->cells[i] = bg_level;
    });
  state->cells[c6_geo_index(state->geo, 0, 0)] = 1.0;
}
