
#include <stdio.h>
#include <mpi.h>
#include <time.h>


int single_send(int rank, int message, int commsize) {

    int ret = 0;
    MPI_Status status = {};

    if (rank == 0) {
        ret = MPI_Send(&message, 1, MPI_INT, rank + 1, 1, MPI_COMM_WORLD);
        if (ret != MPI_SUCCESS) {
            printf("Error executing MPI_Recv.\n");
            return 1;
        }

        // Waiting for the last process in pool of processes, therefore commsize - 1
        ret = MPI_Recv(&message, 1, MPI_INT, commsize - 1, 1, MPI_COMM_WORLD, &status);
        if (ret != MPI_SUCCESS) {
            printf("Error executing MPI_Recv.\n");
            return 1;
        }

        return 0;

    // Slave should first wait for message, then pass it along
    } else {

        ret = MPI_Recv(&message, 1, MPI_INT, rank - 1, 1, MPI_COMM_WORLD, &status);
        if (ret != MPI_SUCCESS) {
            printf("Error executing MPI_Recv.\n");
            return 1;
        }

        ret = MPI_Send(&message, 1, MPI_INT, (rank + 1) % commsize, 1, MPI_COMM_WORLD);
        if (ret != MPI_SUCCESS) {
            printf("Error executing MPI_Recv.\n");
            return 1;
        }

    }


    return 0;

}


int main(int argc, char** argv) {

    int commsize = 0;
    int rank = 0;
    int message = 0;
    int ret = 0;
    MPI_Status status = {0};

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    clock_t start, end;
    double cpu_time_used;
     
    start = clock();

    for (int i = 0; i < 10000; ++i) {
        single_send(rank, message, commsize);
    }
    MPI_Finalize();
    end = clock();

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC / 10000 / commsize;

    printf("Elapsed time for single send: %.10f\n", cpu_time_used);

    return 0;
}