

struct matrix_t {
    
    double** data; 
    int n;      // number of rows
    int m;      // number of cols

};

typedef struct matrix_t matrix_t;


int matrix_init(matrix_t* matrix, int n, int m);

void matrix_dump(matrix_t* matrix);

void matrix_fill(matrix_t* matrix, double* data);




