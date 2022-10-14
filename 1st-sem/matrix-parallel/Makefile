
SOURCES=$(wildcard *.c)

vector: avx.c
	gcc -g  -mavx -march=native -O3 avx.c -o matrix_mult

avx: $(SOURCES)
	gcc -g $^ -mavx -msse4.1 -march=znver1  -DVECTORIZE -o matrix_mult

standart: main.c matrix.c
	gcc -g $^ -o matrix_mult
