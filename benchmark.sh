#!/usr/bin/bash

N=$1

echo "Running sequential version..."
gcc -fopenmp -DNPARALLEL sum.c -o sum
time ./sum $N

echo "Running parallel version..."
gcc -fopenmp sum.c -o sum
time ./sum $N

