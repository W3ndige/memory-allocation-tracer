#ifndef _GNU_SOURCE
#define _GNU_SOURCE

#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>
#include <string.h>

static void __attribute__ ((constructor)) hooks_init(void);

void *malloc(size_t size);
void *calloc(size_t nmemb, size_t size);
void *realloc(void *ptr, size_t size);
void free(void *ptr);

static void __attribute__ ((destructor)) hooks_cleanup(void);


#endif