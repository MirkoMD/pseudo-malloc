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
	allocator->bitmap = bitmap;
	allocator->memory = memory;
	allocator->num_levels = num_levels;
	allocator->min_bucket_size = min_bucket_size;
}

int BuddyAllocator_get_min_level(BuddyAllocator *alloc, int size)
{
	// calculate level for page
	int level = alloc->num_levels - 1;
	// calculate minimum level, using bucket size
	int bucket_size = alloc->min_bucket_size;
	while (bucket_size < size)
	{
		bucket_size <<= 1;
		level--;
	}
	return level;
}

int BuddyAllocator_get_buddy_size(BuddyAllocator *alloc, int level)
{
	return alloc->min_bucket_size << level;
	return 0;
}

void *BuddyAllocator_malloc(BuddyAllocator *alloc, int size)
{
	// get level for page with size bytes
	int level = BuddyAllocator_get_min_level(alloc, size);
}

void BuddyAllocator_free(BuddyAllocator *alloc, void *mem)
{
}