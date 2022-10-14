
#include <immintrin.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define N 110
void matrix_mult(int mat1[N][N], int mat2[N][N], int result[N][N]) {

    for (int i = 0; i < N; i++) {
  for (int j = 0; j < N; j += 8) {
    __m256i sum = _mm256_setzero_si256();
    for (int k = 0; k < N; k++) {
      __m256i bc_mat1 = _mm256_set1_epi32(mat1[i][k]);
       __m256i vec_mat2 = _mm256_loadu_si256((__m256i*)&mat2[k][j]);
      __m256i prod = _mm256_mullo_epi32(bc_mat1, vec_mat2);
      sum = _mm256_add_epi32(sum, prod);
    }
    _mm256_storeu_si256((__m256i*)&result[i][j], sum);
  }
}

}

int matrix_dump(int mat[N][N]) {

    for (int row = 0; row < N; ++row) {
        for (int col = 0; col < N; ++col) {
            printf("%d ", mat[row][col]);
        }
        printf("\n");
    }

    return 0;

}

int main(int argc, char **argv)
{
    int mat1[N][N];
    int mat2[N][N];
    int result[N][N];

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            mat1[i][j] = i + j;
            mat2[i][j] = i + j;
        }
    }
    matrix_mult(mat1, mat2, result);

    // matrix_dump(mat1);
    // printf("\n\n\n");
    // matrix_dump(mat2);
    // printf("\n\n\n");
    matrix_dump(result);

    return 0;
}
