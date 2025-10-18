#include <stddef.h>
#include <stdlib.h>

void *malloc(size_t __size) {
    if (rand() % 100 <= 5) {
        return NULL;
    }
    return malloc(__size);
}
