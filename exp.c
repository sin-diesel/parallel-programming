

#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <mpi.h>



int main(int argc, char** argv) {

    int commsize = 0;
    int rank = 0;
    int message = 0;
    int ret = 0;

    double start_time = 0.0;
    double end_time = 0.0;

    mpf_t partial_sum;
    mpf_t sum;

    // create mpi struct for passing big numbers
    int count = 1;


    mpf_init(partial_sum);
    mpf_init(sum);

    mpf_set_d(partial_sum, 0.0);
    mpf_set_d(sum, 0.0);

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

    mpf_t* result = (mpf_t*) calloc(N, sizeof(mpf_t));
    if (result == NULL) {
        printf("Error in calloc for mpf_t* result\n");
    }

    for (int i = 0; i < N; ++i) {

        mpf_init(result[i]);
        mpf_set_d(result[i], 0.0);

    }

    if (rank == 0) {
        printf("Counting exp with number of terms: %d\n", N);
        fflush(stdout);
        start_time = MPI_Wtime();
    }

    // send the number of terms to compute to children
    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);

    mpf_t factorial;
    mpf_init(factorial);
    mpf_set_d(factorial, 1.0);

    // count factorial. For now can't be made via separate function

    mpf_t fop;
    mpf_init(fop);

    mpz_t iop;
    mpz_init(iop);

    for (int process_id = rank; process_id < N; process_id += commsize) {

        for (int i = 1; i <= process_id; ++i) {

            mpz_set_si(iop, i);
            mpf_set_z(fop, iop);

            mpf_mul(factorial, factorial, fop);

        }

        //gmp_printf("factorial[%d] = %Ff\n", process_id, factorial);
        //fflush(stdout);

        mpf_set_d(fop, 1.0);
        mpf_div(partial_sum, fop, factorial);

        gmp_printf("partial_sum[%d] = %Ff\n", process_id, partial_sum);
        fflush(stdout);

        mpf_add(sum, sum, partial_sum);

        gmp_printf("sum[%d] = %Ff\n", process_id, sum);
        fflush(stdout);

    }

    mpf_clear(factorial);
    mpf_clear(fop);
    mpz_clear(iop);

    ret = MPI_Gather(&sum, 1, MPI_LONG_DOUBLE, result, N, MPI_LONG_DOUBLE, 0, MPI_COMM_WORLD);
    if (ret != MPI_SUCCESS) {
        printf("Error: MPI_Reduce failure\n");
        exit(1);
    }

    end_time = MPI_Wtime();

    double interval = end_time - start_time;

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0) {

        printf("Time taken: %f\n", interval);
        fflush(stdout);
        gmp_printf("Calculated value: %Ff\n", result);
        fflush(stdout);

    }

    mpf_clear(partial_sum);
    mpf_clear(sum);

    for (int i = 0; i < N; ++i) {
        mpf_clear(result[i]);  
    }
    free(result);

    MPI_Finalize();



    return 0;
}
