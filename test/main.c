#include "../src/pseudo_malloc.h"
#include <stdio.h>

#define SEPARATOR "----------------------------------------\n"
#define PAGE_SIZE getpagesize()

void test_pseudo_malloc()
{
    void *ptr;
    for (int size = 1; size <= MEMORY_SIZE * 5; size <<= 1)
    {
        printf("Testing pseudo_malloc() with size: %d\n", size);
        ptr = pseudo_malloc(size);
        printf("Allocated ptr: %p\n", ptr);
        printf("Testing pseudo_free() with ptr: %p\n", ptr);
        pseudo_free(ptr);
        printf("Freed ptr: %p\n", ptr);
        printf(SEPARATOR);
    }
}

void test_pseudo_malloc_mmap()
{
    int NUM_TESTS = 10;
    int SIZE = 1 << 30;
    void *ptr[NUM_TESTS];

    printf("Testing pseudo_malloc() with %d pointers of fixed size: %d\n", NUM_TESTS, SIZE);
    printf("All pointers should be allocated from mmap()\n");
    for (int i = 0; i < NUM_TESTS; i++)
    {
        ptr[i] = pseudo_malloc(SIZE);
        printf("Allocated ptr %d : %p\n", i, ptr[i]);
    }
    printf(SEPARATOR);

    printf("Freeing pointers\n");
    for (int i = 0; i < NUM_TESTS; i++)
    {
        printf("Freeing ptr %d : %p\n", i, ptr[i]);
        pseudo_free(ptr[i]);
    }
    printf(SEPARATOR);
}

void test_pseudo_malloc_buddy_allocator()
{
    int NUM_TESTS = 10;
    int SIZE = PAGE_SIZE / 4 - 1;
    void *ptr[NUM_TESTS];

    printf("Testing pseudo_malloc() with %d pointers of fixed size: %d\n", NUM_TESTS, SIZE);
    printf("All pointers should be allocated from BuddyAllocator\n");
    for (int i = 0; i < NUM_TESTS; i++)
    {
        ptr[i] = pseudo_malloc(SIZE);
        printf("Allocated ptr %d : %p\n", i, ptr[i]);
    }
    printf(SEPARATOR);

    printf("Freeing pointers\n");
    for (int i = 0; i < NUM_TESTS; i++)
    {
        printf("Freeing ptr %d : %p\n", i, ptr[i]);
        pseudo_free(ptr[i]);
    }
    printf(SEPARATOR);
}
int main()
{
    printf(SEPARATOR);
    printf("Testing pseudo_malloc.c\n");
    printf("Page size: %d\n", PAGE_SIZE);
    printf("Requests with size < %d will be handled by BuddyAllocator\n", PAGE_SIZE / 4);
    printf("Requests with size >= %d will be handled by mmap()\n", PAGE_SIZE / 4);
    printf(SEPARATOR);
    pseudo_malloc_init();

    test_pseudo_malloc();
    test_pseudo_malloc_mmap();
    test_pseudo_malloc_buddy_allocator();

    return 0;
}