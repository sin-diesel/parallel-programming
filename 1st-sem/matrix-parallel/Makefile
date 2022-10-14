
SOURCES=$(wildcard *.c)

avx: $(SOURCES)
	gcc -g $^ -mavx -DVECTORIZE -fsanitize=address -o matrix_mult

standart: $(SOURCES)
	gcc -g $^ -fsanitize=address -o matrix_mult
