#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

int operation(int x) {
    return ++x;
}

int main(int argc, char** argv) {
    int my_rank, commsize;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    int message = 0;
    if (my_rank != 0) {
        MPI_Recv(&message, 1, MPI_INT, my_rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("rank %d received %d from %d\n", my_rank, message, my_rank - 1);
        message++;
    }

    MPI_Send(&message, 1, MPI_INT, (my_rank + 1) % commsize, 0, MPI_COMM_WORLD);

    if (my_rank == 0) {
        MPI_Recv(&message, 1, MPI_INT, commsize - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("rank %d received %d from %d\n", my_rank, message, commsize - 1);
    }

    MPI_Finalize();
}
