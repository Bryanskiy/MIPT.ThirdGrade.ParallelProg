#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    if(argc != 2) {
        printf("ERROR: expected 1 argument\n");
        exit(1);
    }

    int my_rank, commsize;

    MPI_Init(&argc, &argv);
    
    int N = atoi(argv[1]);
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    
    MPI_Win win;
    double* local_sum;
    MPI_Alloc_mem(commsize * sizeof(double), MPI_INFO_NULL, &local_sum);
    MPI_Win_create(local_sum, commsize * sizeof(double), sizeof(double), MPI_INFO_NULL, MPI_COMM_WORLD, &win);
    /* calculate result */

    double partitial_ans = 0.0;
    int left_border  = N * my_rank / commsize + 1;
    int right_border = N * (my_rank + 1) / commsize + 1;

    int i = left_border; 
    for(; (i < right_border) && (i <= N); ++i) {
        partitial_ans += 1.0 / i;
    }  
    printf("my rank: %d, left_border: %d, right_border: %d, sum: %f\n", my_rank, left_border, right_border, partitial_ans); 

    MPI_Win_lock(MPI_LOCK_SHARED, my_rank, 0, win);
    if(my_rank != 0) {
        MPI_Put(&partitial_ans, 1, MPI_DOUBLE, 0, my_rank, 1, MPI_DOUBLE, win);
    }
    MPI_Win_unlock(my_rank, win);
    MPI_Barrier(MPI_COMM_WORLD);  
    if(my_rank == 0)
    {
        double result = partitial_ans;
        printf("0 rank stores %f\n", result);
        int i = 1;
        for(; i < commsize; ++i) {
            result += local_sum[i];
            printf("%d rank stores %f\n", i, local_sum[i]);
        }
        printf("result: %f\n", result);
    }

    MPI_Free_mem(local_sum);
    MPI_Finalize();
}
