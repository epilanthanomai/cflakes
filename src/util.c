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
