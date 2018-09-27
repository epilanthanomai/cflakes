#include "cell6.h"

void rsf_init_state(struct c6_state *state, float bg_level) {
  C6_GEO_EACH_CELL(state->geo, r, c,
    {}, {
      state->cells[c6_state_index(state, r, c)] = bg_level;
    }, {});
  state->cells[c6_state_index(state, 0, 0)] = 1.0;
}
