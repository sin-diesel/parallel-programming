

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <immintrin.h>
#include "matrix.h"


int matrix_init(matrix_t* matrix, int rows, int cols) {

    assert(matrix != NULL);
    matrix->rows = rows;
    matrix->cols = cols;

    matrix->data = (double**) calloc(matrix->rows, sizeof(double*));
    assert(matrix->data != NULL);

    for (int row = 0; row < matrix->rows; ++row) {
        matrix->data[row] = (double*) calloc(matrix->cols, sizeof(double));
        assert(matrix->data[row] != NULL);
    }

    return 0;

}

int matrix_dump(matrix_t* matrix) {

    for (int row = 0; row < matrix->rows; ++row) {

        for (int col = 0; col < matrix->cols; ++col) {
            printf("%.2f ", matrix->data[row][col]);
        }

        printf("\n");

    }

    return 0;

}


int matrix_fill(matrix_t* matrix, double* data, int size) {
        
    if (matrix->rows * matrix->cols != size) {
        printf("Error: wrong array size passed to matrix_fill().");
        return -1;
    }
    for (int row = 0; row < matrix->rows; ++row) {
        for (int col = 0; col < matrix->cols; ++col) {
            matrix->data[row][col] = *(data + col + row * matrix->rows);
        }
    }

    return 0;
}

matrix_t matrix_add(matrix_t* lhs, matrix_t* rhs) {

    assert(lhs->rows == rhs->rows);
    assert(lhs->cols == rhs->cols);
    assert(lhs->data != rhs->data);

    matrix_t result;
    matrix_init(&result, lhs->rows, lhs->cols);

    for (int row = 0; row < lhs->rows; ++row) {
        for (int col = 0; col < lhs->cols; ++col) {
            result.data[row][col] = lhs->data[row][col] + rhs->data[row][col];
        }
    }

    return result;
}

matrix_t matrix_mult(matrix_t* lhs, matrix_t* rhs) {

    assert(lhs->cols == rhs->rows);
    assert(lhs->data != rhs->data);

    matrix_t result;
    matrix_init(&result, lhs->rows, rhs->cols);

    double c = 0;
    for (int row_lhs = 0; row_lhs < lhs->rows; ++row_lhs) {
        for (int row_rhs = 0; row_rhs < rhs->rows; ++row_rhs) {

            double elem = lhs->data[row_rhs][row_lhs];
            __m256 factor = _mm256_broadcast_ss(&elem);
            double tmp_mul[8];
            double tmp_add;
            // TODO: Vectorize
            for (int idx = 0; idx < rhs->rows; idx += 8) {
                __m256 first = _mm256_load_ps(&rhs->data[row_lhs][idx]);
                __m256 res = _mm256_mul_ps(first, factor;
                _mm256_store_ps(tmp_mul, res);
                // tmp_add += tmp_mul;
                // result.data[row_rhs][idx] += elem * rhs->data[row_lhs][idx];
            }
        }
    }
    matrix_dump(&result);
}

