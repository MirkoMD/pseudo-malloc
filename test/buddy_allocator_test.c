#include "../src/pseudo_malloc.h"
#include "../src/buddy_allocator.h"
#include "../src/bit_map.h"
#include <sys/mman.h>
#include <stdio.h>

#define BUDDY_LEVELS 9
#define MEMORY_SIZE (1 << 20)
#define MIN_BUCKET_SIZE (MEMORY_SIZE >> (BUDDY_LEVELS - 1))
#define NUM_BUDDIES ((1 << BUDDY_LEVELS) - 1)
#define BITSET_SIZE ((NUM_BUDDIES + 7) << 3)
#define SEPARATOR "--------------------\n"

void test_BuddyAllocator_get_minlevel(BuddyAllocator *alloc)
{
	for (int size = MIN_BUCKET_SIZE; size <= MEMORY_SIZE; size <<= 1)
	{
		printf("Testing BuddyAllocator_get_min_level(), size = %d\n", size);
		printf("Level: %d\n", BuddyAllocator_get_min_level(alloc, size));
	}
}

void test_BuddyAllocator_get_buddy_size(BuddyAllocator *alloc)
{
	printf("Testing BuddyAllocator_get_buddy_size() with level = %d\n", 0);
	printf("Size = %d\n", BuddyAllocator_get_buddy_size(alloc, 0));
	printf("Testing BuddyAllocator_get_buddy_size() with level = %d\n", (BUDDY_LEVELS - 1) / 2);
	printf("Size = %d\n", BuddyAllocator_get_buddy_size(alloc, (BUDDY_LEVELS - 1) / 2));
	printf("Testing BuddyAllocator_get_buddy_size() with level = %d\n", BUDDY_LEVELS - 1);
	printf("Size = %d\n", BuddyAllocator_get_buddy_size(alloc, BUDDY_LEVELS - 1));
}

void test_BuddyAllocator_get_free_buddy_idx(BuddyAllocator *alloc)
{
	printf("Testing BuddyAllocator_get_free_buddy_idx() with level = %d\n", 0);
	printf("Free buddy idx = %d\n", BuddyAllocator_get_free_buddy_idx(alloc, 0));
	printf("Testing BuddyAllocator_get_free_buddy_idx() with level = %d\n", BUDDY_LEVELS - 1);
	printf("Free buddy idx = %d\n", BuddyAllocator_get_free_buddy_idx(alloc, BUDDY_LEVELS - 1));
}

void test_search_free_buddy_at_level(BitMap *bitmap)
{
	printf("Testing search_free_buddy_at_level() with level = %d\n", 0);
	printf("Free buddy idx = %d\n", search_free_buddy_at_level(bitmap, 0));
	printf("Testing search_free_buddy_at_level() with level = %d\n", BUDDY_LEVELS - 1);
	printf("Free buddy idx  = %d\n", search_free_buddy_at_level(bitmap, BUDDY_LEVELS - 1));
}

void test_BuddyAllocator_malloc_free(BuddyAllocator *alloc)
{
	for (int size = 1; size <= MEMORY_SIZE; size <<= 1)
	{
		printf("Testing BuddyAllocator_malloc(), size = %d\n", size);
		void *mem = BuddyAllocator_malloc(alloc, size);
		printf("Allocated memory: %p\n", mem);
		if (mem != NULL)
		{
			printf("Testing BuddyAllocator_free(), size = %d\n", size);
			BuddyAllocator_free(alloc, mem);
			printf("Freed memory: %p\n", mem);
			printf(SEPARATOR);
			continue;
		}
		printf(SEPARATOR);
	}
}

int main()
{
	char memory[MEMORY_SIZE];
	uint8_t buffer[BITSET_SIZE];

	BuddyAllocator alloc;
	BitMap bitmap;
	BitMap_init(&bitmap, NUM_BUDDIES, buffer);
	BitMap_setBit(&bitmap, 0, BUDDY_FREE);
	BuddyAllocator_init(&alloc, &bitmap, memory, BUDDY_LEVELS, MIN_BUCKET_SIZE);

	printf("Buddy levels: %d\n", BUDDY_LEVELS);
	printf("Memory size: %d\n", MEMORY_SIZE);
	printf("Min bucket size: %d\n", MIN_BUCKET_SIZE);
	printf("Num buddies: %d\n", NUM_BUDDIES);
	printf("Bitset size: %d\n", BITSET_SIZE);
	printf(SEPARATOR);

	// test_BuddyAllocator_get_minlevel(&alloc);
	// printf(SEPARATOR);

	// test_BuddyAllocator_get_buddy_size(&alloc);
	// printf(SEPARATOR);

	// test_BuddyAllocator_get_free_buddy_idx(&alloc);
	// printf(SEPARATOR);

	// test_search_free_buddy_at_level(&bitmap);
	// printf(SEPARATOR);1

	test_BuddyAllocator_malloc_free(&alloc);
}
