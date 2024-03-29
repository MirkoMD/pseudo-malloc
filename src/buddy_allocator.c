#include <stdio.h>
#include <assert.h>
#include "buddy_allocator.h"

// returns the index of the level of the tree that contains the given index
int levelIdx(int idx)
{
	return sizeof(int) * 8 - __builtin_clz(idx) - 1;
}

// returns the index of the parent of the node at the given index
int parentIdx(int idx)
{
	return (idx - 1) / 2;
}

// returns the index of the left child of the node at the given index
int leftChildIdx(int idx)
{
	return 2 * idx + 1;
}

// returns the index of the right child of the node at the given index
int rightChildIdx(int idx)
{
	return 2 * idx + 2;
}

// initializes the buddy allocator
void BuddyAllocator_init(BuddyAllocator *allocator, BitMap *bitmap, char *memory, int num_levels, int min_bucket_size)
{
	allocator->bitmap = bitmap;
	allocator->memory = memory;
	allocator->num_levels = num_levels;
	allocator->min_bucket_size = min_bucket_size;
}

// returns the minimum level of the buddy allocator tree that can contain a block of the given size
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

// returns the size of a buddy at the given level
int BuddyAllocator_get_buddy_size(BuddyAllocator *alloc, int level)
{
	return alloc->min_bucket_size * (1 << (alloc->num_levels - level - 1));
}

// returns the bitmap index of the first free buddy at the given level, -1 if not found
int search_free_buddy_at_level(BitMap *bitmap, int level)
{
	// get index of first buddy at level
	int start = (1 << level) - 1;
	// get index of last buddy at level
	int end = (1 << (level + 1)) - 1;

	for (int idx = start; idx < end; idx++)
	{
		// check if buddy is free
		if (BitMap_bit(bitmap, idx) == BUDDY_FREE)
		{
			// return index of buddy
			return idx;
		}
	}
	// no free buddy
	return -1;
}

// returns the index of a free buddy starting from the given level, -1 if not found
int BuddyAllocator_get_free_buddy_idx(BuddyAllocator *alloc, int level)
{
	// base case
	if (level < 0)
	{
		return -1;
	}

	// get index of first buddy at level
	int idx = search_free_buddy_at_level(alloc->bitmap, level);
	// buddy found at level level
	if (idx != -1)
	{
		BitMap_setBit(alloc->bitmap, idx, BUDDY_USED);
		return idx;
	}

	// look for parent buddy
	int parent_idx = BuddyAllocator_get_free_buddy_idx(alloc, level - 1);
	// no available buddy
	if (parent_idx == -1)
	{
		return -1;
	}

	// split parent buddy, set the right one as free and return the left one
	BitMap_setBit(alloc->bitmap, rightChildIdx(parent_idx), BUDDY_FREE);
	return leftChildIdx(parent_idx);
}

// returns the address of the buddy at the given level and index
void *BuddyAllocator_get_address(BuddyAllocator *alloc, int level, int idx)
{
	int idx_in_level = idx - ((1 << level) - 1);
	int offset = idx_in_level * BuddyAllocator_get_buddy_size(alloc, level);
	return (void *)alloc->memory + offset;
}

// allocates a block of memory of the given size
void *BuddyAllocator_malloc(BuddyAllocator *alloc, int size)
{
	// get level for page with size bytes
	int level = BuddyAllocator_get_min_level(alloc, size + sizeof(int));
	// find first available page
	int buddy_idx = BuddyAllocator_get_free_buddy_idx(alloc, level);
	if (buddy_idx == -1)
	{
		printf("No available memory\n");
		return NULL;
	}

	int *buddy_addr = (int *)BuddyAllocator_get_address(alloc, level, buddy_idx);
	// save buddy index in memory
	*buddy_addr = buddy_idx;
	return (void *)(buddy_addr + 1);
}

// frees the buddy at the given index
void BuddyAllocator_free_buddy(BuddyAllocator *alloc, int idx)
{
	assert(idx >= 0 && idx < alloc->bitmap->num_bits);

	BitMap_setBit(alloc->bitmap, idx, BUDDY_FREE);

	int parent_idx, left_child_idx, right_child_idx;
	while (idx != 0)
	{
		parent_idx = parentIdx(idx);
		left_child_idx = leftChildIdx(parent_idx);
		right_child_idx = rightChildIdx(parent_idx);

		// if both children are free, merge them into parent
		if (BitMap_bit(alloc->bitmap, left_child_idx) == BUDDY_FREE && BitMap_bit(alloc->bitmap, right_child_idx) == BUDDY_FREE)
		{
			BitMap_setBit(alloc->bitmap, parent_idx, BUDDY_FREE);
			BitMap_setBit(alloc->bitmap, left_child_idx, BUDDY_USED);
			BitMap_setBit(alloc->bitmap, right_child_idx, BUDDY_USED);
		}
		else // one of the children is used
		{
			break;
		}
		idx = parent_idx;
	}
}

// frees the pointer
void BuddyAllocator_free(BuddyAllocator *alloc, void *mem)
{
	// get buddy index from memory
	int buddy_idx = *((int *)mem - 1);
	BuddyAllocator_free_buddy(alloc, buddy_idx);
}