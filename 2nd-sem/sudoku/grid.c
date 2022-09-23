
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "sudoku_grid.h"

int grid_init(grid* grid, char* filename) {

    FILE* file = fopen(filename, "r");
    assert(file != NULL);

    int grid_size = 0;

    int res = fscanf(file, "%d", &grid_size);
    assert(res == 1);

    grid->size = grid_size;
    int** grid_data = (int**) calloc(grid->size, sizeof(int*));
    for (int i = 0; i < grid->size; ++i) {
        grid_data[i] = (int*) calloc(grid->size, sizeof(int));
    }

    for (int row = 0; row < grid_size; ++row) {
        for (int col = 0; col < grid_size; ++col) {
            fscanf(file,  "%d", &grid_data[row][col]);
        }
    }

    grid->grid = grid_data;

}

void grid_dump(grid* grid) {

    assert(grid != NULL);
    printf("grid: %p\n", grid);
    printf("size: %d\n", grid->size);

    for (int row = 0; row < grid->size; ++row) {
        for (int col = 0; col < grid->size; ++col) {
            printf("%3d ", grid->grid[row][col]);
        }
        printf("\n");
    }

}

void grid_destruct(grid* grid) {

    assert(grid != NULL);

    for (int row = 0; row < grid->size; ++row) {
        free(grid->grid[row]);
    }

    free(grid->grid);

}
