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
#include <stdlib.h>
#include "util.h"

void error(char *msg) {
  perror(msg);
  exit(1);
}

void *must_malloc(size_t sz) {
  void *result = malloc(sz);
  if (result == NULL)
    error("malloc");
  return result;
}

float rand_float(float base, float var) {
  return base + (float) rand() / ((float) RAND_MAX / var);
}
