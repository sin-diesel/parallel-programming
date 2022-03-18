

#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <mpi.h>
#include <string.h>

#define MAX_STR_SIZE 4096
#define DEFAULT_PREC 100

const char* decimal = "0.";

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

    mpf_set_default_prec(DEFAULT_PREC);


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

    char* result = (char*) calloc(MAX_STR_SIZE, sizeof(char));

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

        // gmp_printf("partial_sum[%d] = %.*Ff\n", process_id, DEFAULT_PREC, partial_sum);
        // fflush(stdout);

        mpf_add(sum, sum, partial_sum);

        gmp_printf("sum[%d] = %.*Ff\n", process_id, DEFAULT_PREC, sum);
        fflush(stdout);

    }

    mpf_clear(factorial);
    mpf_clear(fop);
    mpz_clear(iop);

    mp_exp_t exp;

    char* sum_as_str = mpf_get_str(NULL, &exp, 10, 0, sum);
    if (sum_as_str == NULL) {
        printf("Error converting sum to string\n");
        exit(1);
    }

    printf("Resulting exponent: %ld\n", exp);

    int str_len = strlen(sum_as_str);

    printf("sum_as_str: %s\n", sum_as_str);
    printf("sum_as_str len: %d\n", str_len);

    ret = MPI_Send(sum_as_str, str_len, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    if (ret != MPI_SUCCESS) {
        printf("Error: MPI_Send failure\n");
        exit(1);
    }

    ret = MPI_Send(&exp, 1, MPI_LONG, 0, 0, MPI_COMM_WORLD);
    if (ret != MPI_SUCCESS) {
        printf("Error: MPI_Send failure\n");
        exit(1);
    }

    // end_time = MPI_Wtime();

    // double interval = end_time - start_time;

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0) {

        ret = MPI_Recv(result, str_len, MPI_CHAR, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        if (ret != MPI_SUCCESS) {
            printf("Error: MPI_Recv failure\n");
            exit(1);
        }

        printf("Received string: %s\n", result);

        // add 0.1 string to the beginning of converted string
        char* tmp_result = strdup(decimal);
        printf("tmp_result after strdup: %s\n", tmp_result);

        char tmp[MAX_STR_SIZE];

        // пиздец.
        strcpy(tmp, result);
        memset(result, '\0', MAX_STR_SIZE);
        strcpy(result, decimal);
        strcat(result, tmp);

        printf("After this shit: %s\n", result);

        mp_exp_t exp;

        ret = MPI_Recv(&exp, 1, MPI_LONG, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        if (ret != MPI_SUCCESS) {
            printf("Error: MPI_Recv failure\n");
            exit(1);
        }

        printf("Received exponent: %ld\n", exp);


        // try print first element
        mpf_t first_elem;

        ret = mpf_init_set_str(first_elem, result, 10);
        if (ret != 0) {
            printf("Error converting string to sum\n");
            exit(1);
        } 

        //printf("Time taken: %f\n", interval);
        gmp_printf("Calculated value: %Ff\n", first_elem);
        fflush(stdout);

    }

    mpf_clear(partial_sum);
    mpf_clear(sum);
    // free(result);

    MPI_Finalize();


    return 0;
}
