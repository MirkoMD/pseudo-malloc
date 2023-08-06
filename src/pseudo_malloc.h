#pragma once
#include <unistd.h>

#define BUDDY_LEVELS 9
#define MEMORY_SIZE (1 << 20)
#define MIN_BUCKET_SIZE (MEMORY_SIZE >> (BUDDY_LEVELS - 1))
#define NUM_BUDDIES ((1 << BUDDY_LEVELS) - 1)
#define BITSET_SIZE ((NUM_BUDDIES + 7) << 3)

void pseudo_malloc_init();

void *pseudo_malloc(size_t size);

void pseudo_free(void *ptr);