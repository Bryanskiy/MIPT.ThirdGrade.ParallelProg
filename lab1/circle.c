#include <mpi.h>
#include <iostream>
#include <vector>

int main(int argc, char** argv) {
    int my_rank, commsize;

    int N = std::atoi(argv[1]);

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    std::vector<int> vec(10000);
    double time = 0;
    for(int i = 0; i < N; ++i) {
        
        int vecIdx = rand() % vec.size();        
        int* ptr = &vec[vecIdx];

        time -= MPI_Wtime();
        if (my_rank != 0) {
            MPI_Recv(ptr, 1, MPI_INT, my_rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        if (my_rank == 0) {
            MPI_Send(ptr, 1, MPI_INT, my_rank + 1, 0, MPI_COMM_WORLD);
        }
        time += MPI_Wtime();
    }

    if(my_rank == 0) {
        std::cout << "Time: " << time / N << std::endl;
    }

    MPI_Finalize();
}
