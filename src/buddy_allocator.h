#pragma once
#include "bit_map.h"

#define MAX_LEVELS 16

typedef struct
{
	int num_levels;
	char *memory;		 // the memory area to be managed
	int min_bucket_size; // the minimum page of RAM that can be returned
	BitMap *bitmap;
} BuddyAllocator;

// initializes the buddy allocator, and checks that the buffer is large enough
void BuddyAllocator_init(BuddyAllocator *allocator, BitMap *bitmap, char *memory, int num_levels, int min_bucket_size);

// allocates memory
void *BuddyAllocator_malloc(BuddyAllocator *alloc, int size);

// releases allocated memory
void BuddyAllocator_free(BuddyAllocator *alloc, void *mem);