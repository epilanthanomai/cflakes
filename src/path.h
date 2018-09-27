#pragma once

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
void pa_each_segment(struct pa_path*, void (*)(int r, int c, int n, void*), void*);

struct pa_path_set *pa_make_path_list_from_bag(struct pa_segment_bag*);
