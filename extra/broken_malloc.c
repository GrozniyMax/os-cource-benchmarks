#define _GNU_SOURCE
#include <dlfcn.h>
#include <stddef.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

static void* (*real_malloc)(size_t) = NULL;
static int initialized = 0;

void *malloc(size_t size) {
    if (!real_malloc) {
        real_malloc = dlsym(RTLD_NEXT, "malloc");
    }

    if (!initialized) {
        srand(time(NULL));
        initialized = 1;
    }

    if (rand() % 1000000 < 5) {
        return NULL;
    }

    return real_malloc(size);
}