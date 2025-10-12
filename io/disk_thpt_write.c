#include "disk_thpt_write.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>
#include <getopt.h>

void disk_thpt_write_usage() {
    printf("Usage: disk_thpt_write [options]\n");
    printf("Options:\n");
    printf("  -s SIZE    File size in MB (default: 1024)\n");
    printf("  -b SIZE    Block size in KB (default: 64)\n");
    printf("  -d SEC     Test duration in seconds (default: 30)\n");
    printf("  -f FILE    Test file name (default: disk_thpt_test.dat)\n");
    printf("  -c         Disable cache (O_DIRECT)\n");
    printf("  -v         Verbose output\n");
}

static struct option long_options[] = {
    {"size", required_argument, 0, 's'},
    {"block", required_argument, 0, 'b'},
    {"duration", required_argument, 0, 'd'},
    {"file", required_argument, 0, 'f'},
    {"nocache", no_argument, 0, 'c'},
    {"verbose", no_argument, 0, 'v'},
    {0, 0, 0, 0}
};

double get_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1000000.0;
}

disk_thpt_write_args parse(int argc, char **argv) {
    disk_thpt_write_args cfg = {
        .file_size = DEFAULT_FILE_SIZE,
        .block_size = DEFAULT_BLOCK_SIZE,
        .duration = DEFAULT_DURATION,
        .verbose = 0,
        .nocache = 0,
        .filename = FILENAME
    };

    int opt;
    while ((opt = getopt_long(argc, argv, "s:b:d:f:cv", long_options, NULL)) != -1) {
        switch (opt) {
            case 's': cfg.file_size = atol(optarg) * 1024 * 1024; break;
            case 'b': cfg.block_size = atol(optarg) * 1024; break;
            case 'd': cfg.duration = atoi(optarg); break;
            case 'f': cfg.filename = optarg; break;
            case 'c': cfg.nocache = 1; break;
            case 'v': cfg.verbose = 1; break;
            default: break;
        }
    }

    return cfg;
}

int disk_thpt_write_run(disk_thpt_write_args cfg) {
    if (cfg.file_size < cfg.block_size) {
        fprintf(stderr, "Error: file size < block size\n");
        return 1;
    }

    int fd;
    FILE *f = NULL;

    // Открываем файл один раз
    if (cfg.nocache) {
        fd = open(cfg.filename, O_WRONLY | O_CREAT | O_TRUNC | O_DIRECT, 0666);
        if (fd < 0) {
            fprintf(stderr, "O_DIRECT not supported (%s), falling back to normal write\n", strerror(errno));
            cfg.nocache = 0;
        } else {
            // Устанавливаем точный размер файла
            if (ftruncate(fd, cfg.file_size) != 0) {
                perror("ftruncate");
                close(fd);
                return 1;
            }
        }
    }

    if (!cfg.nocache) {
        f = fopen(cfg.filename, "wb");
        if (!f) { perror("fopen"); return 1; }
        if (ftruncate(fileno(f), cfg.file_size) != 0) {
            perror("ftruncate");
            fclose(f);
            return 1;
        }
    }

    // Буфер для записи
    void *buf;
    if (cfg.nocache) {
        if (posix_memalign(&buf, 4096, cfg.block_size) != 0) {
            fprintf(stderr, "posix_memalign failed\n");
            if (cfg.nocache) close(fd); else fclose(f);
            return 1;
        }
    } else {
        buf = malloc(cfg.block_size);
    }
    if (!buf) { perror("malloc"); if (cfg.nocache) close(fd); else fclose(f); return 1; }

    // Заполняем буфер случайными данными
    for (size_t i = 0; i < cfg.block_size; i++) ((char*)buf)[i] = rand() % 256;

    printf("\nDisk Write Throughput Test\n");
    printf("==========================\n");
    printf("File: %s\nBlock: %.1f KB\nDuration: %d s\nCache: %s\n\n",
           cfg.filename, cfg.block_size / 1024.0, cfg.duration,
           cfg.nocache ? "disabled (O_DIRECT)" : "enabled");

    printf("%10s %15s %10s\n", "Time(s)", "Throughput(MB/s)", "IOPS");
    printf("-------------------------------------------\n");

    double start = get_time();
    double next_report = start + 1.0;
    double end = start + cfg.duration;

    size_t total_bytes = 0;
    size_t total_ops = 0;

    off_t offset = 0;

    while (get_time() < end) {
        ssize_t w;
        if (cfg.nocache) {
            w = write(fd, buf, cfg.block_size);
        } else {
            w = fwrite(buf, 1, cfg.block_size, f);
        }

        if (w <= 0) break;

        total_bytes += w;
        total_ops++;
        offset += w;

        // "Круговой" write: если дошли до конца файла, возвращаемся в начало
        if (offset >= cfg.file_size) {
            offset = 0;
            if (cfg.nocache) lseek(fd, 0, SEEK_SET);
            else fseek(f, 0, SEEK_SET);
        }

        double now = get_time();
        if (now >= next_report) {
            double elapsed = now - start;
            double mbps = (total_bytes / (1024.0*1024.0)) / elapsed;
            double iops = total_ops / elapsed;
            printf("%10.1f %15.2f %10.2f\n", elapsed, mbps, iops);
            next_report += 1.0;
        }
    }

    double elapsed = get_time() - start;
    printf("\n=== Results ===\nTotal data written: %.2f MB\nElapsed: %.2f s\nAverage throughput: %.2f MB/s\nAverage IOPS: %.2f\n",
           total_bytes / (1024.0*1024.0), elapsed,
           (total_bytes / (1024.0*1024.0)) / elapsed,
           total_ops / elapsed);

    free(buf);
    if (cfg.nocache) close(fd);
    else fclose(f);

    return 0;
}