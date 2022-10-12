
#include <stdlib.h>
#include <stdio.h>
#include "matrix.h"


int main() {

    matrix_t A;
    matrix_t B;
    float data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
                     11, 12, 13, 14, 15, 16, 17, 18};
    matrix_init(&A, 4, 4);
    matrix_init(&B, 4, 4);

    matrix_fill(&A, data, 16);
    matrix_fill(&B, data, 16);
    matrix_t res = matrix_mult(&A, &B);
    matrix_dump(&res);
    
    return 0;
}


