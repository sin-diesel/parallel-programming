

struct grid_t {

    int** grid;
    int size;

};

struct coord_t {

    int x;
    int y;

};

typedef struct coord_t coord_t;

typedef struct grid_t grid_t;

void grid_init(grid_t* grid, char* filename);

void grid_dump(grid_t* grid);

void grid_destruct(grid_t* grid);

void grid_solve(grid_t* grid);

void grid_elimination(grid_t* grid);

void grid_remove_lone_rangers(grid_t* grid);

void grid_remove_twins(grid_t* grid);

void grid_remove_triplets(grid_t* grid);


