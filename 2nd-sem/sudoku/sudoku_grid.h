#pragma once
#include "list.h"

#define VALUE_EXISTS 1

enum work_directions {

    ROWS = 1,
    COLS = 2

};

struct value_t {

    list_t* values;
    int size;
    int known_value;

};

typedef struct value_t value_t;


struct grid_t {

    value_t** grid;
    int size;

};

struct coord_t {

    int x;
    int y;

};

typedef struct coord_t coord_t;

struct worker_t {

    coord_t start;
    int grid_size_x;
    int grid_size_y;
    int type;

};

typedef struct worker_t worker_t;

typedef struct grid_t grid_t;

void grid_init(grid_t* grid, char* filename);

void grid_dump(grid_t* grid);

void grid_destruct(grid_t* grid);

void grid_solve(grid_t* grid);

void grid_elimination(grid_t* grid, worker_t* worker);

void grid_remove_lone_rangers(grid_t* grid, worker_t* worker);

void grid_remove_twins(grid_t* grid);

void grid_remove_triplets(grid_t* grid);

void worker_init(worker_t* worker, int start_x, int start_y, int worker_type, int grid_size_x, int grid_size_y);

void value_init(value_t* value, int known_value);

worker_t* get_workers(grid_t* grid, int threads_num);


