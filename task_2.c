
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <assert.h>


int main(int argc, char** argv) {

    double result = 0;
    int commsize = 0;
    int my_rank = 0;
    double partial_sum = 0;
    int ret = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Status status = {0};

    if (argc != 2) {
        printf("usage: ./task_2 N\n");
        exit(1);
    }

    int N = atoi(argv[1]);
    printf("Summing from 0 to %d...\n", N);

    int delta = N / commsize;
    assert(delta > 0);

    int start = N / commsize * my_rank + 1;
    assert (start > 0 && start <= N);

    int end = start + delta;

    #ifdef DEBUG
    printf("Process %d starting from %d, ending in %d\n", my_rank, start - 1, end - 1);
    #endif

    for (int i = start; i < end; ++i) {

        #ifdef DEBUG
        printf("i = %d\n", i);
        #endif

        result += 1.0 / i;
    }

    #ifdef DEBUG
    printf("Process %d calculated sum: %lg\n", my_rank, result);
    #endif 

    if (my_rank != 0) {

        ret = MPI_Send(&result, 1, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
        if (ret != MPI_SUCCESS) {
            printf("Error executing MPI_Send.\n");
            return 1;
        }

    } else {

        for (int process_id = 1; process_id < commsize; ++process_id) {

            ret = MPI_Recv(&partial_sum, 1, MPI_DOUBLE, process_id, 1, MPI_COMM_WORLD, &status);

            #ifdef DEBUG
            printf("Partial sum received: %lg\n", partial_sum);
            #endif

            if (ret != MPI_SUCCESS) {
                printf("Error executing MPI_Recv.\n");
                return 1;
            }

        }

        // Add result that was calculated by master as well
        result += partial_sum;
        printf("Master assembled all answers: %lg\n", result);

    }

    MPI_Finalize();

    return 0;
}
