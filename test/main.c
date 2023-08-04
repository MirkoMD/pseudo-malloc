#include "../src/pseudo_malloc.h"
#include <stdio.h>

int main()
{
    pseudo_malloc_init();

    char *ptr = pseudo_malloc(1024);
    printf("Page size / 4: %u\n", getpagesize() / 4U);
    printf("Allocated ptr: %p\n", ptr);
    pseudo_free(ptr);

    ptr = pseudo_malloc(1);
    printf("Allocated ptr: %p\n", ptr);
    pseudo_free(ptr);

    return 0;
}