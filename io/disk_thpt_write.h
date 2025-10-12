//
// Created by maxim on 11.10.2025.
//

#ifndef BENCHS_DISK_THPT_READ_H
#define BENCHS_DISK_THPT_READ_H
#include <stddef.h>

#define DEFAULT_FILE_SIZE (1024 * 1024 * 1024) // 1 GB
#define DEFAULT_BLOCK_SIZE (64 * 1024) // 64 KB
#define DEFAULT_DURATION 30 // 30 seconds
#define FILENAME "disk_thpt_test.dat"
#define O_DIRECT 0

typedef struct {
    size_t file_size;
    size_t block_size;
    int duration;
    int verbose;
    char *filename;
    int nocache;
} disk_thpt_write_args;

void disk_thpt_write_usage();

disk_thpt_write_args parse(int argc, char **argv);

int disk_thpt_write_run(disk_thpt_write_args config);

#endif //BENCHS_DISK_THPT_READ_H