#include <stdio.h>
#include <assert.h>
#include "buddy_allocator.h"

int levelIdx(int idx)
{
	return sizeof(int) * 8 - __builtin_clz(idx) - 1;
}

int parentIdx(int idx)
{
	return (idx - 1) / 2;
}

int leftChildIdx(int idx)
{
	return 2 * idx + 1;
}

int rightChildIdx(int idx)
{
	return 2 * idx + 2;
}

void BuddyAllocator_init(BuddyAllocator *allocator, BitMap *bitmap, char *memory, int num_levels, int min_bucket_size)
{
}

void *BuddyAllocator_malloc(BuddyAllocator *alloc, int size)
{
}

void BuddyAllocator_free(BuddyAllocator *alloc, void *mem)
{
}