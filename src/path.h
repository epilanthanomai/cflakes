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
struct pa_path;
struct pa_path_set;
struct pa_segment_bag;

struct pa_segment_bag *pa_make_segment_bag();
void pa_free_segment_bag(struct pa_segment_bag*);
void pa_bag_add_segment(struct pa_segment_bag*, int row, int col, int neighbor);

struct pa_path_set *pa_make_path_set();
void pa_free_path_set(struct pa_path_set*);

struct pa_path *pa_new_path(struct pa_path_set*);
void pa_each_path(struct pa_path_set*, void (*)(struct pa_path*, void*), void*);
void pa_each_segment(struct pa_path*, void (*)(int r, int c, int n, int d, void*), void*);

struct pa_path_set *pa_make_path_list_from_bag(struct pa_segment_bag*);
