
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char** argv) {

    double start_time = 0;
    double end_time = 0;

    int process_id = 0;
    int num_of_proc = 0;
    int message = 1;

    int ret = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_of_proc);
    MPI_Comm_rank(MPI_COMM_WORLD, &process_id);

    // start timer in both master and slave, finish timer only in master
    start_time = MPI_Wtime();

    if (process_id == 0) {

        ret = MPI_Recv(&message, 1, MPI_INT, 1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        if (ret != MPI_SUCCESS) {
            printf("Error executing MPI_Send\n");
            exit(1);
        }

        end_time = MPI_Wtime();

        double interval = (end_time - start_time) * 1000;

        printf("Message passed in %lg\n", interval);

    } else {

        ret = MPI_Send(&message, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
        if (ret != MPI_SUCCESS) {
            printf("Error executing MPI_Send\n");
            exit(1);
        }

    }

    MPI_Finalize();

    return 0;
}
