

all: main.c sudoku_grid.c list.c
	gcc -g -Wall -Wextra -fopenmp main.c  sudoku_grid.c list.c -o sudoku

list: list.c
	gcc -g -c -Wall -Wextra list.c -o list.o

tests: list.c list_tests.c
	gcc -g -Wall -Wextra list.c list_tests.c -o tests

.PHONY: clean

clean:
	rm -f tests list.o sudoku sudoku_grid.o
