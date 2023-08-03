#pragma once
#include "bit_map.h"

#define BUDDY_USED 0
#define BUDDY_FREE 1

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

// auxiliary functions

// returns the minimum level of the buddy, given size
int BuddyAllocator_get_min_level(BuddyAllocator *alloc, int size);

// returns the idx of the buddy at level level in the bitmap
int search_free_buddy_at_level(BitMap *bitmap, int level);

// returns the index of the first free buddy at level level
int BuddyAllocator_get_free_buddy_idx(BuddyAllocator *alloc, int level);

// returns the size of the buddy at level level
int BuddyAllocator_get_buddy_size(BuddyAllocator *alloc, int level);
