#include "hooks.h"

const size_t ALLOC_ARRAY_SIZE = 100;

alloc_info *alloc_array;

int find_empty_alloc_info() {
    for (size_t i = 0; i < ALLOC_ARRAY_SIZE; i++) {
        if (alloc_array[i].mem_ptr == NULL) {
            return i;
        }
    }

    return -1;
}

void insert_alloc_info(void *ptr, size_t size, char *calle) {
    static size_t alloc_array_index = 0;

    size_t insert_index = alloc_array_index;

    if (alloc_array_index >= ALLOC_ARRAY_SIZE) {
        int empty_slot = find_empty_alloc_info();
        if (empty_slot != -1) {
            insert_index = empty_slot;
        }
    }

    alloc_array[insert_index].calle     = calle;
    alloc_array[insert_index].mem_ptr   = ptr;
    alloc_array[insert_index].mem_size  = size;

}

void remove_alloc_info(void *ptr) {
    for (size_t i = 0; i < ALLOC_ARRAY_SIZE; i++) {
        if (alloc_array[i].mem_ptr == ptr) {
            alloc_array[i].calle    = 0;
            alloc_array[i].mem_ptr  = 0;
            alloc_array[i].mem_size = 0;
        }
    }
}

void check_for_leaks() {
    for (size_t i = 0; i < ALLOC_ARRAY_SIZE; i++) {
        if (alloc_array[i].mem_ptr != 0) {
            fprintf(stderr, "[*] Found leak at %s(%ld) = %p\n", 
                    alloc_array[i].calle,
                    alloc_array[i].mem_size,
                    alloc_array[i].mem_ptr);
        }
    }
}

void *_malloc(size_t size) {
    static void *(*real_malloc)(size_t) = NULL;
    if (!real_malloc) {
        real_malloc = dlsym(RTLD_NEXT, "malloc");
    }

    return real_malloc(size);
}

void *malloc(size_t size) {
    void *return_pointer = _malloc(size);
    insert_alloc_info(return_pointer, size, "malloc");
    fprintf(stderr, "malloc(%ld) = %p\n", size, return_pointer);
    return return_pointer;
}

void *_calloc(size_t nmemb, size_t size) {
    static void *(*real_calloc)(size_t, size_t) = NULL;
    if (!real_calloc) {
        real_calloc = dlsym(RTLD_NEXT, "calloc");
    }
}

void *calloc(size_t nmemb, size_t size) {
    void *return_pointer = _calloc(nmemb, size);
    insert_alloc_info(return_pointer, size, "calloc");
    fprintf(stderr, "calloc(%ld) = %p\n", size, return_pointer);
    return return_pointer;
}

void *_realloc(void *ptr, size_t size) {
    static void *(*real_realloc)(void *, size_t) = NULL;
    if (!real_realloc) {
        real_realloc = dlsym(RTLD_NEXT, "realloc");
    }
}

void *realloc(void *ptr, size_t size) {
    void *return_pointer = _realloc(ptr, size);
    remove_alloc_info(ptr);
    insert_alloc_info(return_pointer, size, "calloc");
    fprintf(stderr, "realloc(%ld) = %p", size, return_pointer);
    return return_pointer;
}


void _free(void *ptr) {
    static void (*real_free)(void *) = NULL;
    if (!real_free) {
        real_free = dlsym(RTLD_NEXT, "free");
    }

    real_free(ptr);
}

void free(void *ptr) {
    fprintf(stderr, "free(%p)\n", ptr);
    remove_alloc_info(ptr);
    _free(ptr);
}

void hooks_init(void) {
    alloc_array = _malloc(ALLOC_ARRAY_SIZE * sizeof(alloc_info));
    memset(alloc_array, 0, ALLOC_ARRAY_SIZE);


    return;
}

void hooks_cleanup(void) {
    check_for_leaks();

    _free(alloc_array);
    return;
}