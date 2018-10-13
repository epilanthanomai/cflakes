#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "cell6.h"
#include "path.h"
#include "reiter.h"
#include "svg.h"
#include "util.h"

static char USAGE[] =
"usage: cflakes [options]\n"
"\n"
"Options:\n"
"\t-n DIMENSION\n"
"\t-i ITERATIONS\n"
"\t-b BACKGROUND_BASE\n"
"\t-B BACKGROUND_VARIATION\n"
"\t-j BACKGROUND_JITTER_BASE\n"
"\t-J BACKGROUND_JITTER_VARIATION\n"
"\t-g GAIN_BASE\n"
"\t-G GAIN_VARIATION\n"
"\t-w NEIGHBOR_WEIGHT_BASE\n"
"\t-W NEIGHBOR_WEIGHT_VARIATION\n"
"\t-s SIZE_BASE\n"
"\t-S SIZE_VARIATION\n";

void usage() {
  fputs(USAGE, stderr);
  exit(1);
}

#define MAX_DIM 200
#define MAX_ITERS 10000

#define BG_BASE 0.3
#define BG_VAR 0.2
#define BG_JITTER_BASE 0.0
#define BG_JITTER_VAR 0.1
#define GAIN_BASE 0.0001
#define GAIN_VAR 0.003
#define NWEIGHT_BASE 0.5
#define NWEIGHT_VAR 1.5
#define SIZE_BASE 2.0
#define SIZE_VAR 1.0

struct args {
  int max_dim;
  int max_iters;
  float bg_base;
  float bg_var;
  float bg_jitter_base;
  float bg_jitter_var;
  float gain_base;
  float gain_var;
  float nweight_base;
  float nweight_var;
  float size_base;
  float size_var;
};

static void init_args(struct args *a) {
  a->max_dim = MAX_DIM;
  a->max_iters = MAX_ITERS;
  a->bg_base = BG_BASE;
  a->bg_var = BG_VAR;
  a->bg_jitter_base = BG_JITTER_BASE;
  a->bg_jitter_var = BG_JITTER_VAR;
  a->gain_base = GAIN_BASE;
  a->gain_var = GAIN_VAR;
  a->nweight_base = NWEIGHT_BASE;
  a->nweight_var = NWEIGHT_VAR;
  a->size_base = SIZE_BASE;
  a->size_var = SIZE_VAR;
}

static void parse_int_opt(char *arg, int* dst) {
  char *endptr;
  *dst = strtol(optarg, &endptr, 10);
  if (*endptr) {
    usage();
  }
}

static void parse_float_opt(char *arg, float* dst) {
  char *endptr;
  *dst = strtof(optarg, &endptr);
  if (*endptr) {
    usage();
  }
}

static void parse_args(int argc, char **argv, struct args *a) {
  int c;

  while ((c=getopt(argc, argv, "n:i:b:B:j:J:g:G:w:W:s:S:")) != -1) {
    switch(c) {
    case 'n':
      parse_int_opt(optarg, &a->max_dim);
      break;
    case 'i':
      parse_int_opt(optarg, &a->max_iters);
      break;
    case 'b':
      parse_float_opt(optarg, &a->bg_base);
      break;
    case 'B':
      parse_float_opt(optarg, &a->bg_var);
      break;
    case 'j':
      parse_float_opt(optarg, &a->bg_jitter_base);
      break;
    case 'J':
      parse_float_opt(optarg, &a->bg_jitter_var);
      break;
    case 'g':
      parse_float_opt(optarg, &a->gain_base);
      break;
    case 'G':
      parse_float_opt(optarg, &a->gain_var);
      break;
    case 'w':
      parse_float_opt(optarg, &a->nweight_base);
      break;
    case 'W':
      parse_float_opt(optarg, &a->nweight_var);
      break;
    case 's':
      parse_float_opt(optarg, &a->size_base);
      break;
    case 'S':
      parse_float_opt(optarg, &a->size_var);
      break;
    default:
      usage();
    }
  }
}

static char SVG_COMMENT[] =
"dimension:  %d\n"
"iterations: %d\n"
"background: %f\n"
"bg jitter:  %f\n"
"gain:       %f\n"
"neighb wt:  %f\n"
"dim:        %s\n";

int main(int argc, char **argv) {
  struct args a;

  srand(time(NULL));
  init_args(&a);
  parse_args(argc, argv, &a);

  struct c6_state_geometry *geo = c6_make_geometry(a.max_dim);
  struct c6_state *state = c6_make_state(geo);

  float bg_level = rand_float(a.bg_base, a.bg_var);
  float bg_jitter = rand_float(a.bg_jitter_base, a.bg_jitter_var);
  float const_gain = rand_float(a.gain_base, a.gain_var);
  float ndiff_weight = rand_float(a.nweight_base, a.nweight_var);
  float svg_dim = rand_float(a.size_base, a.size_var);

  char svg_dim_s[12] = {};
  snprintf(svg_dim_s, sizeof(svg_dim_s)-1, "%8.6fin", svg_dim);

  char svg_comment[1024] = {};
  snprintf(svg_comment, sizeof(svg_comment)-1, SVG_COMMENT,
      a.max_dim, a.max_iters,
      bg_level, bg_jitter, const_gain, ndiff_weight, svg_dim_s);

  rsf_init_state(state, bg_level, bg_jitter);
  state = rsf_state_advance_to_edge(
      state, a.max_iters, const_gain, ndiff_weight);

  struct pa_path_set *ps = rsf_make_traced_path_set(state);
  svg_print_paths(ps, a.max_dim, svg_dim_s, svg_comment);

  pa_free_path_set(ps);
  c6_free_state(state);
  c6_free_geometry(geo);
  return 0;
}
