cmake_minimum_required(VERSION 4.0)
project(benchs C)

set(CMAKE_C_STANDARD 11)

add_executable(benchs
        factorize/factorize.h
        factorize/factorize.c
        io/disk_thpt_write.h
        io/disk_thpt_write.c
        io/main.c
        factorize/main.c)
