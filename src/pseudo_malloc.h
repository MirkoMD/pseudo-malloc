#pragma once
#include <unistd.h>

void pseudo_malloc_init();

void *pseudo_malloc(size_t size);

void pseudo_free(void *ptr, size_t size);