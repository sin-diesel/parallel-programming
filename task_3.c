
#include <stdio.h>
#include <mpi.h>


int main(int argc, char** argv) {

    int commsize = 0;
    int rank = 0;
    int message = 0;
    int ret = 0;
    MPI_Status status = {0};

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    //printf("Running program with %d workers\n", commsize);

    // Run through all process id's


        // Master should first send, then wait until message arrives
    if (rank == 0) {

        printf("Entering master\n");

        ret = MPI_Send(&message, 1, MPI_INT, rank + 1, 1, MPI_COMM_WORLD);
        if (ret != MPI_SUCCESS) {
            printf("Error executing MPI_Recv.\n");
            return 1;
        }

        // Waiting for the last process in pool of processes, therefore commsize - 1
        printf("About to receive message in slave\n");
        ret = MPI_Recv(&message, 1, MPI_INT, commsize - 1, 1, MPI_COMM_WORLD, &status);
        if (ret != MPI_SUCCESS) {
            printf("Error executing MPI_Recv.\n");
            return 1;
        }

        MPI_Finalize();

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

        printf("Process %d received message: %d\n", rank, message);
    }

    MPI_Finalize();

    return 0;
}