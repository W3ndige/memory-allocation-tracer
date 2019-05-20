#ifndef _GNU_SOURCE
#define _GNU_SOURCE

#include <stdio.h>
#include <dlfcn.h>

void *malloc(size_t size) {
    void *(*real_malloc)(size_t) = NULL;
    if (!real_malloc) {
        real_malloc = dlsym(RTLD_NEXT, "malloc");
    }

    void *return_pointer = real_malloc(size);
    fprintf(stderr, "malloc(%ld) = %p\n", size, return_pointer);
    return return_pointer;
}

#endif