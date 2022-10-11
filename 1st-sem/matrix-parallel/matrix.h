

struct matrix_t {
    
    double** data; 
    int rows;
    int cols;

};

typedef struct matrix_t matrix_t;

int matrix_init(matrix_t* matrix, int rows, int cols);

int matrix_dump(matrix_t* matrix);

int matrix_fill(matrix_t* matrix, double* data, int size);

matrix_t matrix_add(matrix_t* lhs, matrix_t* rhs);

matrix_t matrix_mult(matrix_t* lhs, matrix_t* rhs);




