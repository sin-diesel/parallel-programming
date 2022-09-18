
#include <stdio.h>
#include <omp.h>


int main () {

    int elements = 65000000;
    int test_var = 0;

    double start_time = 0.0;
    double end_time = 0.0;
    double time = 0.0;

    start_time = omp_get_wtime();
    #pragma omp parallel for
    for (int i = 0; i < elements; ++i) {
        ++test_var;
    }
    end_time = omp_get_wtime();
    time = end_time - start_time;
    printf("Parallel, default balancing: %lg\n", time);

    start_time = omp_get_wtime();
    #pragma omp parallel for schedule(static, 1)
    for (int i = 0; i < elements; ++i) {
        ++test_var;
    }
    end_time = omp_get_wtime();
    time = end_time - start_time;
    printf("Parallel, static 1 balancing: %lg\n", time);

    start_time = omp_get_wtime();
    #pragma omp parallel for schedule(static, 2)
    for (int i = 0; i < elements; ++i) {
        ++test_var;
    }
    end_time = omp_get_wtime();
    time = end_time - start_time;
    printf("Parallel, static 2 balancing: %lg\n", time);

    start_time = omp_get_wtime();
    #pragma omp parallel for schedule(dynamic, 1)
    for (int i = 0; i < elements; ++i) {
        ++test_var;
    }
    end_time = omp_get_wtime();
    time = end_time - start_time;
    printf("Parallel, dynamic 1 balancing: %lg\n", time);

    start_time = omp_get_wtime();
    #pragma omp parallel for schedule(dynamic, 4)
    for (int i = 0; i < elements; ++i) {
        ++test_var;
    }
    end_time = omp_get_wtime();
    time = end_time - start_time;
    printf("Parallel, dynamic 4 balancing: %lg\n", time);

    start_time = omp_get_wtime();
    #pragma omp parallel for schedule(guided)
    for (int i = 0; i < elements; ++i) {
        ++test_var;
    }
    end_time = omp_get_wtime();
    time = end_time - start_time;
    printf("Parallel, guided balancing: %lg\n", time);

    start_time = omp_get_wtime();
    #pragma omp parallel for schedule(guided, 1)
    for (int i = 0; i < elements; ++i) {
        ++test_var;
    }
    end_time = omp_get_wtime();
    time = end_time - start_time;
    printf("Parallel, guided 1 balancing: %lg\n", time);

    start_time = omp_get_wtime();
    #pragma omp parallel for schedule(guided, 2)
    for (int i = 0; i < elements; ++i) {
        ++test_var;
    }
    end_time = omp_get_wtime();
    time = end_time - start_time;
    printf("Parallel, guided 2 balancing: %lg\n", time);

    start_time = omp_get_wtime();
    for (int i = 0; i < elements; ++i) {
        ++test_var;
    }
    end_time = omp_get_wtime();
    time = end_time - start_time;
    printf("Serial : %lg\n", time);


    return 0;
}