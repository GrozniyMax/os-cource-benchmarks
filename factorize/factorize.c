#include "factorize.h"

#include <stdio.h>
#include <stdlib.h>
void factorize_usage() {
    printf("Usage: factorize [number] [iterations] [verbose]\n");
    printf("Example:\n");
    printf("\tfactorize 0 10 true\n");
}

void run_factorize(factorize_args* args) {
    long long number = args->number;
    for (long long i = 2; i * i <= number; i++) {
        while (number % i == 0) {
            if (args->verbose) {
                fprintf(stdout, "%lld\n", args->number);
            }
            number /= i;
        }
    }
    if (args->verbose && number > 1) {
        fprintf(stdout, "%lld\n", number);
    }
}
