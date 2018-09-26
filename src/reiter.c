#include "cell6.h"

void rsf_init_state(struct c6_state *state, float bg_level) {
  C6_STATE_EACH_CELL(state, r, c, {
      state->cells[c6_state_index(state, r, c)] = bg_level;
    }, {});
  state->cells[c6_state_index(state, 0, 0)] = 1.0;
}
