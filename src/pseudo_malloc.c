#include "pseudo_malloc.h"
#include "buddy_allocator.h"
#include <sys/mman.h>

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
        void *ptr = mmap(NULL, size + sizeof(int), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        *((int *)ptr) = size;
        return ptr + sizeof(int);
    }
}

void pseudo_free(void *ptr)
{
    // check if ptr is in BuddyAllocator addresses pool
    if (ptr >= (void *)memory && ptr < (void *)memory + MEMORY_SIZE * sizeof(char))
    {
        BuddyAllocator_free(&alloc, ptr);
    }
    else
    {
        munmap(ptr, *((int *)ptr));
    }
}