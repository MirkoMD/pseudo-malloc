#include "pseudo_malloc.h"
#include "buddy_allocator.h"
#include <sys/mman.h>

#define BUDDY_LEVELS 9
#define MEMORY_SIZE (1 << 20)
#define MIN_BUCKET_SIZE (MEMORY_SIZE >> (BUDDY_LEVELS - 1))
#define NUM_BUDDIES ((1 << BUDDY_LEVELS) - 1)
#define BITSET_SIZE ((NUM_BUDDIES + 7) << 3)

char memory[MEMORY_SIZE];
uint8_t buffer[BITSET_SIZE];

BuddyAllocator alloc;
BitMap bitmap;

void pseudo_malloc_init()
{
    BitMap_init(&bitmap, NUM_BUDDIES, buffer);
    BitMap_setBit(&bitmap, 0, BUDDY_FREE);
    BuddyAllocator_init(&alloc, &bitmap, memory, BUDDY_LEVELS, MIN_BUCKET_SIZE);
}

void *pseudo_malloc(size_t size)
{
    if (size < (size_t)getpagesize() / 4U)
    {
        return BuddyAllocator_malloc(&alloc, size);
    }
    else
    {
        return mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    }
}

void pseudo_free(void *ptr, size_t size)
{
    if (size < (size_t)getpagesize() / 4U)
    {
        BuddyAllocator_free(&alloc, ptr);
    }
    else
    {
        munmap(ptr, size);
    }
}