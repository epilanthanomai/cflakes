#include <stddef.h>
#include <stdio.h>
#include "uthash.h"

#include "path.h"
#include "util.h"

struct pa_segment {
  int row;
  int col;
  int neighbor;
};

struct pa_segment_node {
  struct pa_segment segment;
  int direction;
  UT_hash_handle hh;
  struct pa_segment_node *next;
};

struct pa_segment_bag {
  struct pa_segment_node *segments;
};

struct pa_path {
  struct pa_segment_node *head;
  struct pa_segment_node *tail;
};

struct pa_path_set_node {
  struct pa_path path;
  struct pa_path_set_node *next;
};

struct pa_path_set {
  struct pa_path_set_node *head;
  struct pa_path_set_node *tail;
};

struct pa_segment_bag *pa_make_segment_bag() {
  struct pa_segment_bag *sb = must_malloc(sizeof(struct pa_segment_bag));
  sb->segments = NULL;
  return sb;
}

void pa_free_segment_bag(struct pa_segment_bag *sb) {
  struct pa_segment_node *segments = sb->segments, *sn, *tmp;
  HASH_ITER(hh, segments, sn, tmp) {
    HASH_DEL(segments, sn);
    free(sn);
  }
  free(sb);
}

void pa_bag_add_segment(struct pa_segment_bag *sb, int row, int col, int neighbor) {
  struct pa_segment_node *sn = must_malloc(sizeof(struct pa_segment_node));
  sn->segment.row = row;
  sn->segment.col = col;
  sn->segment.neighbor = neighbor;
  HASH_ADD(hh, sb->segments, segment, sizeof(struct pa_segment), sn);
}

struct pa_path_set *pa_make_path_set() {
  struct pa_path_set *ps = must_malloc(sizeof(struct pa_path_set));
  ps->head = NULL;
  ps->tail = NULL;
  return ps;
}

void pa_free_path_set(struct pa_path_set *ps) {
  struct pa_path_set_node *sn, *next_psn;
  for (sn = ps->head; sn != NULL; sn = next_psn) {
    struct pa_segment_node *pas, *next_pas;
    for (pas = sn->path.head; pas != NULL; pas = next_pas) {
      next_pas = pas->next;
      free(pas);
    }
    next_psn = sn->next;
    free(sn);
  }
  free(ps);
}

struct pa_path *pa_new_path(struct pa_path_set *ps) {
  struct pa_path_set_node *sn = must_malloc(sizeof(struct pa_path_set_node));
  sn->path.head = NULL;
  sn->path.tail = NULL;
  sn->next = NULL;

  if (ps->tail) {
    ps->tail->next = sn;
  } else {
    ps->head = sn;
  }
  ps->tail = sn;

  return &sn->path;
}

void pa_each_path(struct pa_path_set *ps, void (*f)(struct pa_path*, void*), void *data) {
  struct pa_path_set_node *sn;
  for (sn=ps->head; sn != NULL; sn = sn->next) {
    f(&sn->path, data);
  }
}

void pa_each_segment(struct pa_path *pa, void (*f)(int, int, int, int, void*), void *data) {
  struct pa_segment_node *pas;
  for (pas=pa->head; pas != NULL; pas = pas->next) {
    f(pas->segment.row, pas->segment.col, pas->segment.neighbor, pas->direction, data);
  }
}

static void pa_path_append_from_bag(struct pa_path *pa, struct pa_segment_bag *sb, struct pa_segment_node *sn, int direction) {
  HASH_DEL(sb->segments, sn);
  sn->direction = direction;
  sn->next = NULL;
  if (pa->tail) {
    pa->tail->next = sn;
  } else {
    pa->head = sn;
  }
  pa->tail = sn;
}

//         -1, 0
//   0,-1         -1,+1
//          0, 0
//  +1,-1          0,+1
//         +1, 0

static struct candidate_path {
  int row_d;
  int col_d;
  int neighbor;
  int direction;
} candidate_paths[] = {
  // neighbor 0 is to the north, between 0,0 and -1,0
  //   direction 0 is east toward -1,+1. the two options are:
  {  0,  0, 1, 0 },  // turn right. 0,0 neighbor 1 (northeast) heading southeast
  { -1, +1, 2, 0 },  // turn left. -1,+1 neighbor 2 (northwest) heading northeast
  //   direction 1 is west toward 0,-1. the two options are:
  {  0, -1, 1, 1 },  // turn right. 0,-1 neighbor 1 (northeast) heading northwest
  {  0,  0, 2, 1 },  // turn left. 0,0 neighbor 2 (northwest) heading southwest
  // neighbor 1 is to the northeast, between 0,0 and -1,+1
  //   direction 0 is southeast toward 0,+1. the two options are:
  {  0, +1, 2, 1 },  // turn right. 0,+1 neighbor 2 (northwest) heading southwest
  {  0, +1, 0, 0 },  // turn left. 0,+1 neighbor 0 (north) heading east
  //   direction 1 is northwest toward -1,0. the two options are:
  { -1, +1, 2, 0 },  // turn right. -1,+1 neighbor 2 (northwest) heading northeast
  {  0,  0, 0, 1 },  // turn left. 0,0 neighbor 0 (north) heading west
  // neighbor 2 is to the northwest, between 0,0 and 0,-1
  //   direction 0 is northeast toward -1,0. the two options are:
  {  0,  0, 0, 0 },  // turn right. 0,0 neighbor 0 (north) heading east
  {  0, -1, 1, 1 },  // turn left. 0,-1 neighbor 1 (northeast) heading northwest
  //   direction 1 is to the southwest toward +1,-1. the two options are:
  { +1, -1, 0, 1 },  // turn right. +1,-1 neighbor 0 (north) heading west
  { +1, -1, 1, 0 },  // turn right. +1,-1 neighbor 1 (northeast) heading southeast
};


static void pa_advance_segment(struct pa_segment_node **sn, int *direction, struct pa_segment_bag *sb) {
  int candidate_path_idx = 
    (*sn)->segment.neighbor * 4 +  // 4 outgoing candidates per neighbor
    *direction * 2;  // 2 outgoing candidates per direction
  struct candidate_path *turn_right = candidate_paths + candidate_path_idx;
  struct candidate_path *turn_left = candidate_paths + candidate_path_idx + 1;

  struct pa_segment candidate_segment;
  struct pa_segment_node *found;
  int next_direction;
  candidate_segment.row = (*sn)->segment.row + turn_right->row_d;
  candidate_segment.col = (*sn)->segment.col + turn_right->col_d;
  candidate_segment.neighbor = turn_right->neighbor;
  next_direction = turn_right->direction;
  HASH_FIND(hh, sb->segments, &candidate_segment, sizeof(struct pa_segment), found);

  if (found == NULL) {
    candidate_segment.row = (*sn)->segment.row + turn_left->row_d;
    candidate_segment.col = (*sn)->segment.col + turn_left->col_d;
    candidate_segment.neighbor = turn_left->neighbor;
    next_direction = turn_left->direction;
    HASH_FIND(hh, sb->segments, &candidate_segment, sizeof(struct pa_segment), found);
  }

  *sn = found;
  *direction = next_direction;
}

struct pa_path_set *pa_make_path_list_from_bag(struct pa_segment_bag *sb) {
  struct pa_path_set *ps = pa_make_path_set();
  struct pa_segment_node *sn;
  struct pa_path *pa;
  int direction;

  while (sb->segments != NULL) {
    pa = pa_new_path(ps);
    sn = sb->segments;
    direction = 0;
    do {
      pa_path_append_from_bag(pa, sb, sn, direction);
      pa_advance_segment(&sn, &direction, sb);
    } while (sn != NULL);
  }

  return ps;
}
