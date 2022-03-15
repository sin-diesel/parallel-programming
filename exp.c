

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

unsigned long long factorial(int n) {

    unsigned long long res = 1;

    for (int i = 1; i <= n; ++i) {
        res *= i;
    }

    return res;
}

int main(int argc, char** argv) {

    int commsize = 0;
    int rank = 0;
    int message = 0;
    int ret = 0;

    double start_time = 0.0;
    double end_time = 0.0;

    long double partial_sum = 0.0;
    long double sum = 0.0;
    long double result = 0.0;

    MPI_Status status = {0};

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);

    printf("commsize: %d\n", commsize);
    fflush(stdout);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (argc != 2) {
        printf("usage: ./a.out <precision>\n");
        exit(1);
    }

    int N = atoi(argv[1]);

    if (rank == 0) {
        printf("Counting exp with number of terms: %d\n", N);
        fflush(stdout);
        start_time = MPI_Wtime();
    }

    // send the number of terms to compute to children
    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = rank; i < N; i += commsize) {

        partial_sum = 1.0 / factorial(i);

        printf("partial_sum[%d] = %Lf\n", i, partial_sum);
        fflush(stdout);

        sum += partial_sum;

    }

    MPI_Reduce(&sum, &result, 1, MPI_LONG_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    end_time = MPI_Wtime();

    double interval = end_time - start_time;

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Time taken: %f\n", interval);
        fflush(stdout);
        printf("Calculated value: %.10Lf\n", result);
        fflush(stdout);
    }

    MPI_Finalize();

    return 0;
}
