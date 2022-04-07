#include <stdio.h>
#include <assert.h>
#include <stdlib.h>


struct grid_t {

    int K;
    int M;
    int k;
    int m;

    long double tau;
    long double h;

    long double** grid;
};


long double f(int k, int m) {
    return 0.0;
}

void dump_grid(struct grid_t* grid);

void grid_init(int rows, int cols, int num_of_rows, int num_of_cols, struct grid_t* grid) {

    grid->K = rows;
    grid->M = cols;

    grid->k = num_of_rows;
    grid->m = num_of_cols;

    grid->tau = 1.0 * rows / num_of_rows;
    grid->h = 1.0 * cols / num_of_cols;

    grid->grid = (long double**) calloc(grid->K, sizeof(long double*));
    assert(grid->grid != NULL);

    for (int col = 0; col < grid->M; ++col) {
        grid->grid[col] = (long double*) calloc(grid->M, sizeof(long double));
    }

}

long double border_conditions(int cell) {
    return (long double) cell;
}

long double initial_conditions(int cell) {
    return (long double) cell;
}

void fill_border_conditions(struct grid_t* grid) {

    assert(grid != NULL);
    for (int coord = 0; coord < grid->M; ++coord) {
        grid->grid[0][coord] = border_conditions(coord);
    }

}

void fill_initial_conditions(struct grid_t* grid) {

    assert(grid != NULL);
    for (int time = 0; time < grid->K; ++time) {
        grid->grid[time][0] = initial_conditions(time);
    }

}

long double corner_compute(struct grid_t* grid, int k , int m) {
    return 1 / grid->h * (grid->tau * grid->h * f(k, m) + grid->grid[k][m] * grid->h - grid->grid[k][m] * grid->tau + grid->grid[k][m - 1] * grid->tau);
}

long double cross_compute(struct grid_t* grid, int k, int m) {
    return 1 / (2 * grid->tau) * (4 * f(k, m) * grid->tau * grid->h + 2 * grid->tau * grid->grid[k][m - 1] - 2 * grid->h * \
           (grid->grid[k + 1][m] - grid->grid[k - 1][m]));
}

void fill_first_layer(struct grid_t* grid) {

    for (int time = 0; time < grid->K - 1; ++time) {
        grid->grid[time + 1][1] = corner_compute(grid, time + 1, 1);
    }

}

void dump_grid(struct grid_t* grid) {

    for (int time = 0; time < grid->K; ++time) {

        for (int coord = 0; coord < grid->M; ++coord) {
            printf("%6.4Lg ", grid->grid[grid->K - 1 - time][coord]);
        }
        printf("\n");

    }

    printf("\n\n\n");
}


void fill_grid(struct grid_t* grid) {

    for (int coord_layer = 2; coord_layer < grid->M; ++coord_layer) {
        for (int time_layer = 1; time_layer < grid->K - 1; ++time_layer) {

            grid->grid[time_layer][coord_layer] = cross_compute(grid, time_layer, coord_layer);
            dump_grid(grid);

        }

        grid->grid[grid->K - 1][coord_layer] = corner_compute(grid, grid->K - 1, coord_layer);
    }

    grid->grid[grid->K - 1][grid->M - 1] = corner_compute(grid, grid->K - 2, grid->M - 2);

}


int main() {

    struct grid_t grid;
    grid_init(10, 10, 700, 1000, &grid);

    fill_border_conditions(&grid);
    fill_initial_conditions(&grid);

    fill_first_layer(&grid);

    fill_grid(&grid);

    dump_grid(&grid);

    return 0;
}