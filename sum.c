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
    
    /* calculate result  */
    double local_sum = 0.0;

    int left_border  = N * my_rank / commsize + 1;
    int right_border = N * (my_rank + 1) / commsize + 1;

    int i = left_border; 
    for(; (i < right_border) && (i <= N); ++i) {
        local_sum += 1.0 / i;
    }  
    printf("my rank: %d, left_border: %d, right_border: %d, sum: %f\n", my_rank, left_border, right_border, local_sum); 

    /* send/recieve data */
    if(my_rank) {
        int res = MPI_Send(&local_sum, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    } 
    else {
        double ans = local_sum;
        int i = 1;
        for(; i < commsize; ++i) {
            MPI_Recv(&local_sum, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            ans += local_sum;
        }
        
        printf("Result: %f\n", ans);
    }

    MPI_Finalize();
}
