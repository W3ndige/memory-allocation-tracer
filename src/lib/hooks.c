#include "hooks.h"

void hooks_init(void) {

    return;
}

void *malloc(size_t size) {
    static void *(*real_malloc)(size_t) = NULL;
    if (!real_malloc) {
        real_malloc = dlsym(RTLD_NEXT, "malloc");
    }

    void *return_pointer = real_malloc(size);
    fprintf(stderr, "malloc(%ld) = %p\n", size, return_pointer);
    return return_pointer;
}

void *calloc(size_t nmemb, size_t size) {
    static void *(*real_calloc)(size_t, size_t) = NULL;
    if (!real_calloc) {
        real_calloc = dlsym(RTLD_NEXT, "calloc");
    }
    
    void *return_pointer = real_calloc(nmemb, size);
    fprintf(stderr, "calloc(%ld) = %p\n", size, return_pointer);
    return return_pointer;
}

void *realloc(void *ptr, size_t size) {
    static void *(*real_realloc)(void *, size_t) = NULL;
    if (!real_realloc) {
        real_realloc = dlsym(RTLD_NEXT, "realloc");
    }
    
    void *return_pointer = real_realloc(ptr, size);
    fprintf(stderr, "realloc(%ld) = %p", size, return_pointer);
    return return_pointer;
}


void free(void *ptr) {
    static void (*real_free)(void *) = NULL;
    if (!real_free) {
        real_free = dlsym(RTLD_NEXT, "free");
    }

    fprintf(stderr, "free(%p)\n", ptr);
    real_free(ptr);
}

void hooks_cleanup(void) {

    return;
}