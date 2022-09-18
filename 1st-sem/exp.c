

#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <mpi.h>
#include <string.h>

#define MAX_STR_SIZE 10000

//#define DEBUG

// This must be appended before converted as THERE IS NO OTHER WAY AROUND
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

    int count = 1;

    int N = atoi(argv[1]);

    mpf_set_default_prec(N);

    // create mpi struct for passing big numbers
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

    int current_precision = 0;
    int process_id = rank;

    while (current_precision < N) {

        for (int i = 1; i <= process_id; ++i) {

            mpz_set_si(iop, i);
            mpf_set_z(fop, iop);

            mpf_mul_ui(factorial, factorial, i);

        }

        process_id += commsize;

        #ifdef DEBUG

        gmp_printf("factorial[%d] = %Ff\n", process_id, factorial);
        fflush(stdout);

        #endif

        mpf_set_d(fop, 1.0);
        mpf_div(partial_sum, fop, factorial);

        #ifdef DEBUG

        gmp_printf("partial_sum[%d] = %.*Ff\n", process_id, N, partial_sum);
        fflush(stdout);

        #endif

        mpf_add(sum, sum, partial_sum);

        #ifdef DEBUG
        
        gmp_printf("sum[%d] = %.*Ff\n", process_id, N, sum);
        fflush(stdout);

        #endif

        current_precision++;
        //printf("current_precision: %d\n", current_precision);

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

    int str_len = strlen(sum_as_str);

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

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0) {

        mpf_t mpf_result;
        mpf_init(mpf_result);
        mpf_set_d(mpf_result, 0.0);

        // receive message from all processes
        for (int process_id = 0; process_id < commsize; ++process_id) {

            mp_exp_t exp;

            ret = MPI_Recv(result, str_len, MPI_CHAR, process_id, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            if (ret != MPI_SUCCESS) {
                printf("Error: MPI_Recv failure\n");
                exit(1);
            }

            ret = MPI_Recv(&exp, 1, MPI_LONG, process_id, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            if (ret != MPI_SUCCESS) {
                printf("Error: MPI_Recv failure\n");
                exit(1);
            }

            #ifdef DEBUG

            printf("Received string: %s\n", result);
            printf("Received exponent: %ld\n", exp);

            #endif

            // add 0.1 string to the beginning of converted string
            char* tmp_result = strdup(decimal);

            char tmp[MAX_STR_SIZE];

            // пиздец.
            strcpy(tmp, result);
            memset(result, '\0', MAX_STR_SIZE);
            strcpy(result, decimal);
            strcat(result, tmp);

            mpf_t mpf_tmp;

            ret = mpf_init_set_str(mpf_tmp, result, 10);
            if (ret != 0) {
                printf("Error converting string to mpf type\n");
                exit(1);
            } 

            #ifdef DEBUG

            printf("After this shit: %s\n", result);
            gmp_printf("After this shit with gmp_printf: %Ff\n", mpf_tmp);

            #endif

            if (exp == 1) {

                mpf_mul_ui(mpf_tmp, mpf_tmp, 10);

                #ifdef DEBUG
                gmp_printf("mpf_tmp after mul: %Ff\n", mpf_tmp);
                #endif

            }


            mpf_add(mpf_result, mpf_result, mpf_tmp);

            free(tmp_result);


        }

        gmp_printf("Trahatsa zhopa penis blinchiki: %.*Ff\n", N, mpf_result);

        mpf_clear(mpf_result);

    }   

    mpf_clear(partial_sum);
    mpf_clear(sum);

    free(result);

    MPI_Finalize();


    return 0;
}
