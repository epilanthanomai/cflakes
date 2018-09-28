#include <stdio.h>
#include "path.h"
#include "svg.h"

char SVG_HEADER[] =
"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
"<svg width=\"8in\" height=\"8in\" viewBox=\"%d,%d,%d,%d\"\n"
"     xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">\n";

char SVG_FOOTER[] = "</svg>";

char PATH_HEADER[] =
"<path stroke=\"%s\" stroke-width=\"0.1\" fill=\"none\"\n"
"      d=\"\n";

char PATH_FOOTER[] = "\"/>\n";


char *path_colors[] = {
  "black",
  "blue"
};

const float HALF_SIDE = 0.28867513459481287;  // sqrt(3)/6

const float ROW_HEIGHT = 1.0;
const float ROW_OFFSET = 0.5;
const float COL_WIDTH = 3.0 * HALF_SIDE;

static struct segment_points {
  float start_x;
  float start_y;
  float end_x;
  float end_y;
} segment_points[] = {
  // neighbor 0 is north
  { -HALF_SIDE, -0.5, HALF_SIDE, -0.5 },  // direction 0 is east
  { HALF_SIDE, -0.5, -HALF_SIDE, -0.5 },  // direction 1 is west
  // neighbor 1 in northeast
  { HALF_SIDE, -0.5, 2.0 * HALF_SIDE, 0.0 },  // direction 0 is southeast
  { 2.0 * HALF_SIDE, 0.0, HALF_SIDE, -0.5 },  // direction 1 is northwest
  // neighbor 2 is northwest
  { -2.0 * HALF_SIDE, 0.0, -HALF_SIDE, -0.5 },  // direction 0 is northeast
  { -HALF_SIDE, -0.5, -2.0 * HALF_SIDE, 0.0 },  // direction 1 is southwest
};

void svg_get_segment_start_coords(float *x, float *y, int row, int col, int neighbor, int dir) {
  struct segment_points *off = segment_points + (neighbor * 2 + dir);
  *x = COL_WIDTH * col + off->start_x;
  *y = ROW_HEIGHT * row + col * ROW_OFFSET + off->start_y;
}

void svg_get_segment_end_coords(float *x, float *y, int row, int col, int neighbor, int dir) {
  struct segment_points *off = segment_points + (neighbor * 2 + dir);
  *x = COL_WIDTH * col + off->end_x;
  *y = ROW_HEIGHT * row + col * ROW_OFFSET + off->end_y;
}

void svg_print_segment(int row, int col, int neighbor, int dir, void *data) {
  int *segment_counter = (int*)data;
  float x, y;
  if ((*segment_counter)++ == 0) {
    svg_get_segment_start_coords(&x, &y, row, col, neighbor, dir);
    printf("M %f %f\n", x, y);
  }
  svg_get_segment_end_coords(&x, &y, row, col, neighbor, dir);
  printf("L %f %f\n", x, y);
}

void svg_print_path(struct pa_path *pa, void *data) {
  int *path_counter = (int*)data;
  char *path_color = path_colors[(*path_counter)++ > 0];
  int segment_counter = 0;

  printf(PATH_HEADER, path_color);
  pa_each_segment(pa, svg_print_segment, &segment_counter);
  puts(PATH_FOOTER);
}

void svg_print_paths(struct pa_path_set *ps, int max_dim) {
  int path_counter = 0;
  int xmin=0, xmax=2*max_dim, ymin=max_dim/2, ymax=5*max_dim/2;

  printf(SVG_HEADER, xmin, ymin, xmax, ymax);
  pa_each_path(ps, svg_print_path, &path_counter);
  puts(SVG_FOOTER);
}
