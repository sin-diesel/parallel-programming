
#include <check.h>
#include "matrix.h"


START_TEST (test_fill) {

    double data[] = {2.3, 1.5, 3.2,\
                     1.5, 1.2, 1.5,\
                     0,   4.5, 2.1};

    matrix_t matrix;
    matrix_init(&matrix, 3, 3);

    assert(matrix->rows == 3);
    assert(matrix->cols == 3);

    matrix_fill(&matrix, data, sizeof(data) / sizeof(double));
    matrix_dump(&matrix);

}
END_TEST


START_TEST () {

    double data_1[] = {2.3, 1.5, 3.2,\
                     1.5, 1.2, 1.5,\
                     0,   4.5, 2.1};
;

    double data_2[] = {2.3, 1.5, 3.2,\
                     1.5, 1.2, 1.5,\
                     0,   4.5, 2.1};

    matrix_t matrix_1;
    matrix_init(&matrix_1, 3, 3);
    matrix_fill(&matrix_1, data_1, sizeof(data_1) / sizeof(double));
    matrix_dump(&matrix_1)

    matrix_t matrix_2;
    matrix_init(&matrix_2, 3, 3);
    matrix_fill(&matrix_2, data_2, sizeof(data_2) / sizeof(double));
    matrix_dump(&matrix_2);

    matrix_t added = matrix_add(&matrix_1, &matrix_2);
    matrix_dump(&added);

}
END_TEST
