
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

void worker_init(worker_t* worker, int start_x, int start_y, int worker_type, int grid_size_x, int grid_size_y) {

    coord_t start;
    start.x = start_x;
    start.y = start_y;

    worker->start = start;
    worker->grid_size_x = grid_size_x;
    worker->grid_size_y = grid_size_y;
    worker->type = worker_type;

}

worker_t* get_workers(grid_t* grid, int threads_num) {

    int half_threads = threads_num / 2;
    worker_t* workers = (worker_t*) calloc(threads_num, sizeof(coord_t*));
    assert(workers != NULL);

    // Make diagonal starts
    int split_size = grid->size / half_threads;

    for (int thread_num = 0; thread_num < half_threads - 1; ++thread_num) {
        worker_init(&workers[thread_num], thread_num * split_size, thread_num * split_size, ROWS, split_size, split_size);
    }

    // last rows worker gets remaining portion of work
    worker_init(&workers[half_threads - 1], (half_threads - 1) * split_size, (half_threads - 1) * split_size, 
                                                                             ROWS,
                                                                             grid->size - (half_threads - 1) * split_size,
                                                                             grid->size - (half_threads - 1) * split_size);


    // Place column workers on remaining places to contol column
    split_size = (grid->size - workers[0].grid_size_x) / (half_threads - 1);
    printf("split size for cols workers: %d\n", split_size);
    int thread_ordered_num = 1;
    for (int thread_num = half_threads; thread_num < threads_num - 2; ++thread_num) {
        printf("thread_num: %d\n", thread_num);
        worker_init(&workers[thread_num], thread_ordered_num * split_size, 0, COLS, split_size, split_size);
        ++thread_ordered_num;
    }

    // last - 1 cols worker gets remaining portion of work
    worker_init(&workers[threads_num - 2], (half_threads - 1) * split_size, 0,
                                                                            COLS,
                                                                            grid->size - (half_threads - 1) * split_size ,
                                                                            grid->size - (half_threads - 1) * split_size);
    // last cols worker gets last unchecked cols
    worker_init(&workers[threads_num - 1], 0, workers[0].grid_size_y, COLS, workers[0].grid_size_x, workers[1].grid_size_y);
    return workers;
}

void grid_solve(grid_t* grid) {

    int workers_count = 0;
    #pragma omp parallel
    {
    #pragma omp flush(workers_count)
    workers_count = omp_get_num_threads();
    }

    printf("workers count: %u\n", workers_count);
    worker_t* workers = get_workers(grid, workers_count);
    for (int worker_idx = 0; worker_idx < workers_count; ++worker_idx) {

        printf("Worker %d info:\n", worker_idx);
        printf("Starting point: %d %d\n", workers[worker_idx].start.x, workers[worker_idx].start.y);
        printf("Grid size x: %d\n", workers[worker_idx].grid_size_x);
        printf("Grid size y: %d\n", workers[worker_idx].grid_size_y);
        printf("Worker type: %d\n", workers[worker_idx].type);
        printf("\n\n\n");

    }

    #pragma omp parallel
    {

    int worker_num = omp_get_thread_num();
    int worker_grid_size = grid->size / workers_count;

    if (worker_num == workers_count - 1) {
        worker_grid_size = worker_grid_size + grid->size % workers_count;
    }

    printf("workers num: %u\n", worker_num);
    printf("workers_grid_size: %u\n", worker_grid_size);

    }
    // grid_remove_lone_rangers(grid);
    // grid_remove_twins(grid);
    // grid_remove_triplets(grid);

}

