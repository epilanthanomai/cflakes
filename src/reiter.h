#pragma once

struct c6_state;
struct rsf_machine;
struct pa_path_set;


struct rsf_machine *rsf_make_machine(struct c6_state*);
void rsf_free_machine(struct rsf_machine*);
void rsf_ma_calculate_next(struct rsf_machine*);
int rsf_ma_next_any_edges_ice(struct rsf_machine*);
struct c6_state *rsf_ma_advance(struct rsf_machine*);

void rsf_init_state(struct c6_state*, float bg_level);
struct c6_state *rsf_state_advance_once(struct c6_state*);
struct c6_state *rsf_state_advance_to_edge(struct c6_state*, int max_iters);
void rsf_state_dump_ice(struct c6_state*);

struct pa_path_set *rsf_make_traced_path_set(struct c6_state*);
