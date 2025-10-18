#!/bin/bash

max_attempts=1000000
attempt=1

while [ $attempt -le $max_attempts ]; do
    echo "Попытка $attempt из $max_attempts"
    LD_PRELOAD=./libmalloc_broken.so java HelloWorld

    exit_code=$?
    if [ $exit_code -ne 0 ]; then
        echo "Ошибка! Код возврата: $exit_code"
        break
    fi

    attempt=$((attempt + 1))
    sleep 2
done