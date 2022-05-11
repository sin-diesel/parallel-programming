

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "matrix.h"


int matrix_init(matrix_t* matrix, int n, int m) {

    assert(matrix != NULL);
    matrix->n = n;
    matrix->m = m;

    matrix->data = (double**) calloc(n, sizeof(double*));
    assert(matrix->data != NULL);

    for (int row = 0; row < matrix->n; ++row) {
        matrix->data[row] = (double*) calloc(m, sizeof(double));
        assert(matrix->data[row] != NULL);
    }

    return 0;

}

void matrix_dump(matrix_t* matrix) {

    for (int row = 0; row < matrix->n; ++row) {

        for (int col = 0; col < matrix->m; ++col) {
            printf("%.4f", matrix->data[row][col]);
        }

        printf("\n");

    }

}


void matrix_fill(matrix_t* matrix, double* data) {

}
