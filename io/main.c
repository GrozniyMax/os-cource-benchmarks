#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "disk_thpt_write.h"

typedef struct {
    int id;
    disk_thpt_write_args config;
} thread_arg_t;

void *thread_func(void *arg) {
    thread_arg_t *targ = (thread_arg_t *)arg;
    disk_thpt_write_args cfg = targ->config;

    // Чтобы избежать конфликтов при параллельных записях
    char filename[256];
    snprintf(filename, sizeof(filename), "%s_%d.dat", cfg.filename, targ->id);
    cfg.filename = filename;

    if (cfg.verbose) {
        printf("[Thread %d] Starting test on %s\n", targ->id, cfg.filename);
    }

    disk_thpt_write_run(cfg);

    if (cfg.verbose) {
        printf("[Thread %d] Finished\n", targ->id);
    }

    return NULL;
}

int main(int argc, char **argv) {
    disk_thpt_write_args cfg = parse(argc, argv);
    int threads = 1;

    // Обработка дополнительного аргумента -t
    for (int i = 1; i < argc; i++) {
        if ((strcmp(argv[i], "-t") == 0 || strcmp(argv[i], "--threads") == 0) && i + 1 < argc) {
            threads = atoi(argv[++i]);
        }
    }

    if (threads <= 0) threads = 1;

    printf("Running disk throughput test with %d thread%s\n",
           threads, threads > 1 ? "s" : "");

    pthread_t *tids = malloc(sizeof(pthread_t) * threads);
    thread_arg_t *targs = malloc(sizeof(thread_arg_t) * threads);

    if (!tids || !targs) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    for (int i = 0; i < threads; i++) {
        targs[i].id = i;
        targs[i].config = cfg;

        int rc = pthread_create(&tids[i], NULL, thread_func, &targs[i]);
        if (rc != 0) {
            fprintf(stderr, "Failed to create thread %d\n", i);
            threads = i; // only join created ones
            break;
        }
    }

    for (int i = 0; i < threads; i++) {
        pthread_join(tids[i], NULL);
    }

    free(tids);
    free(targs);

    printf("\nAll threads finished.\n");
    return 0;
}
