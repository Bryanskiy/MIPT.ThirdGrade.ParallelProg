#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>

uint64_t factorial(int n) {
    assert(n >= 0);

    uint64_t ans = 1;
    for(int i = 1; i <= n; ++i) {
        ans *= i;
    }

    return ans;
}

int main(int argc, char** argv) {
    if(argc != 2) {
        printf("ERROR: expected 1 argument\n");
        exit(0);
    }

    int rank, commsize;
    int n = n = atoi(argv[1]);

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    long double partialSum = 0;
    for(int i = rank; i < n; i += commsize) {
        partialSum += 1. / factorial(i);
    }


#ifdef DEBUG
    printf("rank: %d, partial sum = %Lf\n", rank, partialSum);
#endif

    if(rank) {
        int res = MPI_Send(&partialSum, 1, MPI_LONG_DOUBLE, 0, 0, MPI_COMM_WORLD);
    } 
    else {
        long double ans = partialSum;
        int i = 1;
        for(; i < commsize; ++i) {
            MPI_Recv(&partialSum, 1, MPI_LONG_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            ans += partialSum;
        }
        
        printf("Result: %Lf\n", ans);
    }
    MPI_Finalize();
    return 0;
}
