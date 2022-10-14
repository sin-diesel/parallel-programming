
SOURCES=$(wildcard *.c)

avx: $(SOURCES)
	gcc -g $^ -mavx -msse4.1 -march=native -DVECTORIZE -o matrix_mult

standart: $(SOURCES)
	gcc -g $^ -fsanitize=address -o matrix_mult
