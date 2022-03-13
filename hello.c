#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    int my_rank, commsize;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    printf("Hello world! communication size: %d, My rank: %d\n", commsize, my_rank);
    MPI_Finalize();
}
