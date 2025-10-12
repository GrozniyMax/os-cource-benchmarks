//
// Created by maxim on 11.10.2025.
//

#ifndef BENCHS_FACTORIZE_H
#define BENCHS_FACTORIZE_H
#include <stdbool.h>

typedef struct {
    int iterations;
    long long number;
    bool verbose;
} factorize_args;

void factorize_usage();

void run_factorize(factorize_args* args);

#endif //BENCHS_FACTORIZE_H