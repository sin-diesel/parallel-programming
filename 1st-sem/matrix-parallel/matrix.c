

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <immintrin.h>
#include <smmintrin.h>
#include <tmmintrin.h>
#include "matrix.h"


int matrix_init(matrix_t* matrix, int rows, int cols) {

    assert(matrix != NULL);
    matrix->rows = rows;
    matrix->cols = cols;

    matrix->data = (int**) calloc(matrix->rows, sizeof(int*));
    assert(matrix->data != NULL);

    for (int row = 0; row < matrix->rows; ++row) {
        matrix->data[row] = (int*) calloc(matrix->cols, sizeof(int));
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
            printf("%8d ", matrix->data[row][col]);
        }
        printf("\n");
    }

    return 0;

}


int matrix_fill(matrix_t* matrix, int size) {
        
    if (matrix->rows * matrix->cols != size) {
        printf("Error: wrong array size passed to matrix_fill().");
        return -1;
    }
    for (int row = 0; row < matrix->rows; ++row) {
        for (int col = 0; col < matrix->cols; ++col) {
            matrix->data[row][col] = row + col; 
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

    #ifdef VECTORIZE

    __m256i vec_multi_res = _mm256_setzero_si256();
    __m256i vec_lhs = _mm256_setzero_si256();
    __m256i vec_rhs = _mm256_setzero_si256();

    #endif
    for (int row_lhs = 0; row_lhs < lhs->rows; ++row_lhs) {
        for (int row_rhs = 0; row_rhs < rhs->rows; ++row_rhs) {

            #ifdef VECTORIZE

            vec_lhs = _mm256_set1_epi32(lhs->data[row_rhs][row_lhs]);

            for (int idx = 0; idx < rhs->rows; idx += 8) {

                vec_rhs = _mm256_loadu_si256((__m256i*)&rhs->data[row_rhs][idx]);
                vec_multi_res = _mm256_loadu_si256((__m256i*)&result.data[row_lhs][idx]);
                vec_multi_res = _mm256_add_epi32(vec_multi_res ,_mm256_mullo_epi32(vec_lhs, vec_rhs));
                _mm256_storeu_si256((__m256i*)&result.data[row_lhs][idx], vec_multi_res); 

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

