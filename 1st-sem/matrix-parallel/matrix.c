

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <immintrin.h>
#include "matrix.h"


int matrix_init(matrix_t* matrix, int rows, int cols) {

    assert(matrix != NULL);
    matrix->rows = rows;
    matrix->cols = cols;

    matrix->data = (float**) _mm_malloc(matrix->rows * sizeof(float*), 32);
    assert(matrix->data != NULL);

    for (int row = 0; row < matrix->rows; ++row) {
        matrix->data[row] = (float*) _mm_malloc(matrix->cols * sizeof(float), 32);
        assert(matrix->data[row] != NULL);
    }

    return 0;

}

int matrix_destroy(matrix_t* matrix) {
    for (int row = 0; row < matrix->rows; ++row) {
        free(matrix->data[row]);
    }
    free(matrix->data);
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


int matrix_fill(matrix_t* matrix, float* data, int size) {
        
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


// Vectorize
// Align memory
// test
matrix_t matrix_mult(matrix_t* lhs, matrix_t* rhs) {

    assert(lhs->cols == rhs->rows);
    assert(lhs->data != rhs->data);

    matrix_t result;
    matrix_init(&result, lhs->rows, rhs->cols);

    float c = 0;
    for (int row_lhs = 0; row_lhs < lhs->rows; ++row_lhs) {
        for (int row_rhs = 0; row_rhs < rhs->rows; ++row_rhs) {

            #ifdef VECTORIZE

            float elem = lhs->data[row_rhs][row_lhs];
            __m256 factor = _mm256_broadcast_ss(&elem);
            float* tmp_mul = (float*) _mm_malloc(8 * sizeof(float), 32);
            for (int idx = 0; idx < rhs->rows; idx += 8) {

                __m256 rhs_data = _mm256_load_ps(&rhs->data[row_lhs][idx]);
                __m256 res_data = _mm256_load_ps(&result.data[row_rhs][idx]);

                __m256 res_mul = _mm256_mul_ps(rhs_data, factor);

                __m256 res_add = _mm256_add_ps(res_mul, res_data);
                _mm256_store_ps(&result.data[row_rhs][idx], res_add);

            }

            #else

            double elem = lhs->data[row_rhs][row_lhs];
            for (int idx = 0; idx < rhs->rows; ++idx) {
                result.data[row_rhs][idx] += elem * rhs->data[row_lhs][idx];
            }

            #endif
        }
    }
    return result;
}

