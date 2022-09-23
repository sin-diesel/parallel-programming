

struct grid_t {

    int** grid;
    int size;

};

typedef struct grid_t grid;

int grid_init(grid* grid, char* filename);

void grid_dump(grid* grid);

void grid_destruct(grid* grid);

