#pragma once

/* Copyright 2018, Hope Ranker
 *
 * This file is part of cflakes.
 *
 * cflakes is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * cflakes is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with cflakes. If not, see <https://www.gnu.org/licenses/>.
 */

struct c6_state;
struct rsf_machine;
struct pa_path_set;


struct rsf_machine *rsf_make_machine(struct c6_state*);
void rsf_free_machine(struct rsf_machine*);
void rsf_ma_calculate_next(struct rsf_machine*);
int rsf_ma_next_any_edges_ice(struct rsf_machine*);
struct c6_state *rsf_ma_advance(struct rsf_machine*);

void rsf_init_state(struct c6_state*, float bg_level, float bg_jitter);
struct c6_state *rsf_state_advance_once(
    struct c6_state*, float const_gain, float ndiff_weight);
struct c6_state *rsf_state_advance_to_edge(
    struct c6_state*, int max_iters, float const_gain, float ndiff_weight);
void rsf_state_dump_ice(struct c6_state*);

struct pa_path_set *rsf_make_traced_path_set(struct c6_state*);
