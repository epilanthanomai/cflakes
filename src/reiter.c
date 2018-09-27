#include <stdio.h>
#include <stdlib.h>
#include "cell6.h"
#include "path.h"
#include "util.h"

#define CONST_GAIN 0.001

struct rsf_machine {
  struct c6_state_geometry *geo;
  struct c6_state *current;
  struct c6_state *next;
  struct c6_state *receptive;
  struct c6_state *diffusion;
};

struct rsf_machine *rsf_make_machine(struct c6_state *s) {
  struct rsf_machine *m = must_malloc(sizeof(struct rsf_machine));
  m->geo = s->geo;
  m->current = s;
  m->next = c6_make_state(s->geo);
  m->receptive = c6_make_state(s->geo);
  m->diffusion = c6_make_state(s->geo);
  return m;
}

void rsf_free_machine(struct rsf_machine *m) {
  free(m->next);
  free(m->receptive);
  free(m->diffusion);
  free(m);
}

static void rsf_ma_calculate_receptive(
    struct c6_state *dest,
    struct c6_state *src,
    struct c6_state_geometry *geo) {
  C6_GEO_EACH_CELL(geo, i, {
      int receptive = (src->cells[i] >= 1);
      for (int n=0; !receptive && n < geo->cells[i].num_neighbors; n++) {
        int ni = geo->cells[i].neighbors[n].index;
        if (src->cells[ni] >= 1.0) {
          receptive = 1;
        }
      }
      dest->cells[i] = receptive ? (src->cells[i] + CONST_GAIN) : 0.0;
    });
}

static void rsf_ma_calculate_diffusion(
    struct c6_state *dest,
    struct c6_state *src,
    struct c6_state *receptive,
    struct c6_state_geometry *geo) {
  C6_GEO_EACH_CELL(geo, i, {
      float cell_receptive = (receptive->cells[i] > 0.0) ? 0.0 : src->cells[i];
      float neighbor_sum = 0;
      int num_neighbors = geo->cells[i].num_neighbors;
      for (int n=0; n < num_neighbors; n++) {
        int ni = geo->cells[i].neighbors[n].index;
        if (receptive->cells[ni] == 0.0) {
          neighbor_sum += src->cells[ni];
        }
      }
      float neighbor_avg = neighbor_sum / num_neighbors;
      dest->cells[i] = (cell_receptive + neighbor_avg) / 2.0;
    });
}

static void rsf_ma_sum_components(
    struct c6_state *dest,
    struct c6_state *receptive,
    struct c6_state *diffusion,
    struct c6_state_geometry *geo) {
  C6_GEO_EACH_CELL(geo, i, {
      dest->cells[i] = receptive->cells[i] + diffusion->cells[i];
    });
}

void rsf_ma_calculate_next(struct rsf_machine *m) {
  rsf_ma_calculate_receptive(m->receptive, m->current, m->geo);
  rsf_ma_calculate_diffusion(m->diffusion, m->current, m->receptive, m->geo);
  rsf_ma_sum_components(m->next, m->receptive, m->diffusion, m->geo);
}

int rsf_ma_next_any_edges_ice(struct rsf_machine *m) {
  struct c6_state *s = m->next;
  for (int e=0; e < s->geo->num_edges; e++) {
    int ei = s->geo->edges[e];
    if (s->cells[ei] >= 1.0) {
      return 1;
    }
  }
  return 0;
}

struct c6_state *rsf_ma_advance(struct rsf_machine *m) {
  struct c6_state *temp = m->current;
  m->current = m->next;
  m->next = temp;
  return m->current;
}

void rsf_init_state(struct c6_state *state, float bg_level) {
  C6_GEO_EACH_CELL(state->geo, i, {
      state->cells[i] = bg_level;
    });
  state->cells[c6_geo_index(state->geo, 0, 0)] = 1.0;
}

struct c6_state *rsf_state_advance_once(struct c6_state *state) {
  struct rsf_machine *m = rsf_make_machine(state);
  rsf_ma_calculate_next(m);
  struct c6_state *result = rsf_ma_advance(m);
  rsf_free_machine(m);
  return result;
}

struct c6_state *rsf_state_advance_to_edge(struct c6_state *state, int max_iters) {
  struct rsf_machine *m = rsf_make_machine(state);
  int i;
  for (i = 0; i < max_iters; i++) {
    rsf_ma_calculate_next(m);
    if (rsf_ma_next_any_edges_ice(m)) {
      break;
    }
    state = rsf_ma_advance(m);
  }
  return state;
}

void rsf_state_dump_ice(struct c6_state *s) {
  C6_GEO_EACH_ROW_CELL(s->geo, r, c, i,
    {
      for (int j = r; j < s->geo->center; j++) {
        fputc(' ', stderr);
      }
    }, {
      fputc(s->cells[i]>=1.0 ? '*' : ' ', stderr);
    }, {
      fputc('\n', stderr);
    });
}

struct pa_path_set *rsf_make_traced_path_set(struct c6_state *s) {
  struct pa_segment_bag *sb = pa_make_segment_bag();

  C6_GEO_EACH_ROW_CELL(s->geo, r, c, i,
    {}, {
      int layout_col = c + s->geo->rows[r].col_offset;
      int node_is_ice = s->cells[i] >= 1.0;
      for (int n=0; n < s->geo->cells[i].num_neighbors; n++) {
        int neighbor_num = s->geo->cells[i].neighbors[n].neighbor_num;
        if (neighbor_num < MAX_NEIGHBORS / 2) {
          int neighbor_index = s->geo->cells[i].neighbors[n].index;
          int neighbor_is_ice = s->cells[neighbor_index] >= 1.0;
          if (node_is_ice != neighbor_is_ice) {
            pa_bag_add_segment(sb, r, layout_col, neighbor_num);
          }
        }
      }
    }, {});

  struct pa_path_set *ps = pa_make_path_list_from_bag(sb);
  pa_free_segment_bag(sb);
  return ps;
}
