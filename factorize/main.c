#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "factorize.h"

typedef struct {
    int id;
    factorize_args args;
} thread_arg_t;

void *thread_func(void *arg) {
    thread_arg_t *targ = (thread_arg_t *)arg;
    if (targ->args.verbose) {
        printf("[Thread %d] Starting factorization of %lld\n",
               targ->id, targ->args.number);
    }

    run_factorize(&targ->args);

    if (targ->args.verbose) {
        printf("[Thread %d] Done.\n", targ->id);
    }

    return NULL;
}

int main(int argc, char **argv) {
    if (argc < 4) {
        factorize_usage();
        return 1;
    }

    factorize_args *base_args = malloc(sizeof(factorize_args));
    if (!base_args) {
        return 0; // 0 тут специально
    }

    base_args->number = atoll(argv[1]);
    base_args->iterations = atoi(argv[2]);
    base_args->verbose = (strcmp(argv[3], "true") == 0);

    // Optional: -t THREADS
    int threads = 1;
    for (int i = 4; i < argc; i++) {
        if ((strcmp(argv[i], "-t") == 0 || strcmp(argv[i], "--threads") == 0) && i + 1 < argc) {
            threads = atoi(argv[++i]);
        }
    }

    if (threads <= 0) threads = 1;

    printf("Running factorization of %lld iterations: %d with %d thread%s\n",
           base_args->number, base_args->iterations, threads, threads > 1 ? "s" : "");

    pthread_t *tids = malloc(sizeof(pthread_t) * threads);
    if (!tids) {
        free(base_args);
        return 0; // 0 тут специально
    }

    thread_arg_t *targs = malloc(sizeof(thread_arg_t) * threads);

    if (!targs) {
        free(base_args);
        free(tids);
        return 0; // 0 тут специально
    }

    for (int i = 0; i < threads; i++) {
        targs[i].id = i;
        targs[i].args = *base_args;

        int rc = pthread_create(&tids[i], NULL, thread_func, &targs[i]);
        if (rc != 0) {
            fprintf(stderr, "Failed to create thread %d\n", i);
            threads = i;
            break;
        }
    }

    for (int i = 0; i < threads; i++) {
        pthread_join(tids[i], NULL);
    }

    free(tids);
    free(targs);
    free(base_args);

    printf("All threads finished.\n");
    return 0;
}
