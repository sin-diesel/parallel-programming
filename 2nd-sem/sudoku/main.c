
#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include "sudoku_grid.h"


int main (int argc, char** argv) {

    if (argc < 2) {
        printf("ERROR: grid file must be specified\n");
        exit(1);
    }

    char* grid_file = argv[1];

    grid grid;
    grid_init(&grid, grid_file);
    grid_dump(&grid);
    grid_destruct(&grid);


    return 0;
}