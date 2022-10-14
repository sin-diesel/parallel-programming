
#include <stdlib.h>
#include <stdio.h>
#include "matrix.h"


int main() {

    matrix_t A;
    matrix_t B;

    int rows = 100;
    int cols = 100;
    int matrix_size = rows * cols;

    float* data = (float*) calloc(matrix_size, sizeof(float));
    for (int idx = 0; idx < matrix_size; ++idx) {
        data[idx] = idx;
    }

    matrix_init(&A, rows, cols);
    matrix_init(&B, rows, cols);

    matrix_fill(&A, data, matrix_size);
    matrix_fill(&B, data, matrix_size);
    matrix_t res = matrix_mult(&A, &B);
    #ifndef VECTORIZE
    matrix_dump(&res);
    #endif
    //matrix_dump(&res);

    matrix_destroy(&A);
    matrix_destroy(&B);
    //matrix_destroy(&res);

    free(data);

    return 0;
}


