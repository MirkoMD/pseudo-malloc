#include "pseudo_malloc.h"
#include "buddy_allocator.h"
#include <sys/mman.h>

#define BUFFER_SIZE 102400
#define BUDDY_LEVELS 9
#define MEMORY_SIZE (1024 * 1024)
#define MIN_BUCKET_SIZE (MEMORY_SIZE >> (BUDDY_LEVELS))

char buffer[BUFFER_SIZE]; // 100 Kb buffer to handle memory should be enough
char memory[MEMORY_SIZE];

BuddyAllocator alloc;

void pseudo_malloc_init()
{
    BuddyAllocator_init(&alloc, BUDDY_LEVELS, buffer, BUFFER_SIZE, memory, MIN_BUCKET_SIZE);
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