
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <omp.h>
#include "sudoku_grid.h"

void grid_init(grid_t* grid, char* filename) {

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

void grid_dump(grid_t* grid) {

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

void grid_destruct(grid_t* grid) {

    assert(grid != NULL);

    for (int row = 0; row < grid->size; ++row) {
        free(grid->grid[row]);
    }

    free(grid->grid);

}

/* Workers layoyt:
  1 - 3: rows checkers
  4 - 6: cols checkers

  1 1 1 5 5 5 6 6 6
  1 1 1 5 5 5 6 6 6
  1 1 1 5 5 5 6 6 6
  4 4 4 2 2 2 0 0 0
  4 4 4 2 2 2 0 0 0
  4 4 4 2 2 2 0 0 0
  0 0 0 3 3 3 0 0 0
  0 0 0 3 3 3 0 0 0
  0 0 0 3 3 3 0 0 0 
*/
void grid_solve(grid_t* grid) {

    #pragma omp parallel
    {
    unsigned workers_count = omp_get_num_threads();
    unsigned worker_num = omp_get_thread_num();
    unsigned worker_grid_size = grid->size / workers_count;

    if (worker_num == workers_count - 1) {
        worker_grid_size = worker_grid_size + grid->size % workers_count;
    }

    printf("workers count: %u\n", workers_count);
    printf("workers num: %u\n", worker_num);
    printf("workers_grid_size: %u\n", worker_grid_size);

    }



    //grid_elimination(grid);
    // grid_remove_lone_rangers(grid);
    // grid_remove_twins(grid);
    // grid_remove_triplets(grid);

}

// void grid_elimination(grid_t* grid) {

//     unsigned workers_count = omp_get_num_threads();
//     unsigned worker_num = omp_get_thread_num();
//     unsigned workers_grid_size = grid->size / 3; // hardcoded, remove after verifying that algorithms works

//     for (int row = 0; row < workers_grid_size; ++row) {
//         for (int col = 0;)
//     }

// }
