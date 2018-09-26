#pragma once

#include <stddef.h>

void error(char *msg) __attribute__((noreturn));
void *must_malloc(size_t);
