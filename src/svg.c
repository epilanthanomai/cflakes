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

#include <stdio.h>
#include "path.h"
#include "svg.h"

static char SVG_HEADER[] =
"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
"<svg width=\"%s\" height=\"%s\" viewBox=\"%f,%f,%f,%f\"\n"
"     xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\"\n"
"     stroke-width=\"0.1\" fill=\"none\">\n"
"<!--\n"
"%s"
"-->\n"
"\n";

static char SVG_FOOTER[] = "</svg>";

static char PATH_HEADER[] =
"<path stroke=\"%s\"\n"
"      d=\"\n";

static char PATH_FOOTER[] = "\"/>\n";

static char BORDER_CIRCLE[] =
"<circle cx=\"0\" cy=\"0\" r=\"%f\" stroke=\"black\"/>\n\n";

static char *path_colors[] = {
  "purple",
  "blue"
};

#define BORDER_MULTIPLIER 1.05
#define VIEW_MULTIPLIER 1.1
#define HALF_SIDE 0.28867513459481287  // sqrt(3)/6

#define ROW_HEIGHT 1.0
#define ROW_OFFSET 0.5
#define COL_WIDTH (3.0 * HALF_SIDE)

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

static void svg_get_hex_center_coords(float *x, float *y, int row, int col) {
  *x = COL_WIDTH * col;
  *y = ROW_HEIGHT * row + col * ROW_OFFSET;
}

static void svg_get_segment_start_coords(float *x, float *y, int row, int col, int neighbor, int dir) {
  struct segment_points *off = segment_points + (neighbor * 2 + dir);
  svg_get_hex_center_coords(x, y, row, col);
  *x +=  off->start_x;
  *y +=  off->start_y;
}

static void svg_get_segment_end_coords(float *x, float *y, int row, int col, int neighbor, int dir) {
  struct segment_points *off = segment_points + (neighbor * 2 + dir);
  *x = COL_WIDTH * col + off->end_x;
  *y = ROW_HEIGHT * row + col * ROW_OFFSET + off->end_y;
}

static void svg_print_segment(int row, int col, int neighbor, int dir, void *data) {
  int *segment_counter = (int*)data;
  float x, y;
  if ((*segment_counter)++ == 0) {
    svg_get_segment_start_coords(&x, &y, row, col, neighbor, dir);
    printf("M %f %f\n", x, y);
  }
  svg_get_segment_end_coords(&x, &y, row, col, neighbor, dir);
  printf("L %f %f\n", x, y);
}

static void svg_print_path(struct pa_path *pa, void *data) {
  int *path_counter = (int*)data;
  char *path_color = path_colors[(*path_counter)++ > 0];
  int segment_counter = 0;

  printf(PATH_HEADER, path_color);
  pa_each_segment(pa, svg_print_segment, &segment_counter);
  puts(PATH_FOOTER);
}

static void pa_print_circle(int max_dim) {
  float r;
  r = (float) max_dim * BORDER_MULTIPLIER;
  printf(BORDER_CIRCLE, r);
}

void svg_print_paths(struct pa_path_set *ps, int max_dim, char *svg_dim, char *comment) {
  int path_counter = 0;
  float view_dim = (float) max_dim * VIEW_MULTIPLIER;

  printf(SVG_HEADER, svg_dim, svg_dim, -view_dim, -view_dim, 2.0 * view_dim, 2.0 * view_dim, comment);
  pa_each_path(ps, svg_print_path, &path_counter);
  pa_print_circle(max_dim);
  puts(SVG_FOOTER);
}
