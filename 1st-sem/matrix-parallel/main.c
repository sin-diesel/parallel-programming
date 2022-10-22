
#include <stdlib.h>
#include <stdio.h>
#include "matrix.h"


int main() {

    matrix_t A;
    matrix_t B;

    int rows = 512;
    int cols = 512  ;
    int matrix_size = rows * cols;

    matrix_init(&A, rows, cols);
    matrix_init(&B, rows, cols);

    matrix_fill(&A, matrix_size);
    matrix_fill(&B, matrix_size);
    matrix_t res = matrix_mult(&A, &B);
    #ifndef VECTORIZE
    matrix_dump(&res);
    #endif
    matrix_dump(&res);

    matrix_destroy(&A);
    matrix_destroy(&B);
    //matrix_destroy(&res);

    //free(data);

    return 0;
}


