#pragma once

struct rsf_geometry;
struct rsf_state;

struct rsf_geometry *rsf_make_geometry(int max_dim);
void rsf_free_geometry(struct rsf_geometry*);

struct rsf_state *rsf_make_state(struct rsf_geometry*, float bg_level);
void rsf_free_state(struct rsf_state*);

void rsf_dump_state(struct rsf_state*);
