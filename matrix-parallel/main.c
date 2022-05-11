
#include <stdlib.h>
#include <stdio.h>
#include "matrix.h"




int main() {

    double data[] = {2.3, 1.5, 3.2,\
                     1.5, 1.2, 1.5,\
                     0,   4.5, 2.1};

    matrix_t matrix;
    matrix_init(&matrix, 3, 3);
    matrix_fill(&matrix, data, sizeof(data) / sizeof(double));
    matrix_dump(&matrix);

    return 0;
}


