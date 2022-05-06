#include <vector>
#include <iostream>
#include <mpi.h>
#include "matrix.hpp"

using gtype = double;

gtype phi(gtype x) {
    return x;
}

gtype psi(gtype t) {
    return t;
}

gtype f(gtype t, gtype x) {
    return t + x;
}

void angle(matrix::matrix_t<gtype>& grid, std::size_t k, std::size_t m, gtype tau, gtype h) {
    grid[k-1][m] = f(k * tau, m * h) * tau - (grid[k][m] - grid[k][m+1]) / h * tau + grid[k][m];
}

void krest(matrix::matrix_t<gtype>& grid, std::size_t k, std::size_t m, gtype tau, gtype h) {
    grid[k-1][m] = f(k * tau, m * h) * 2 * tau - (grid[k][m-1] - grid[k][m+1]) * tau / h + grid[k+1][m];
}

void initGrid(matrix::matrix_t<gtype>& grid, gtype tau, gtype h) {
    std::size_t K = grid.get_rows_number();
    std::size_t M = grid.get_cols_number();

    for(std::size_t i = 0; i < M; ++i) {
        grid[K-1][i] = phi(i * h);
    }

    for(std::size_t i = 0; i < K; ++i) {
        grid[K - 1 - i][0] = psi(i * tau);
    }
}

void initLayer(matrix::matrix_t<gtype>& grid, gtype tau, gtype h) {
    std::size_t K = grid.get_rows_number();
    std::size_t M = grid.get_cols_number();
    for(std::size_t i = 1; i < M; ++i) {
        angle(grid, K - 1, i, tau, h);
    }
}


int main(int argc, char** argv) {
    std::size_t K, M; 
    gtype tau, h; 

    K = std::atoi(argv[1]);
    M = std::atoi(argv[2]);
    tau = std::atof(argv[3]);
    h = std::atof(argv[4]);

    matrix::matrix_t<gtype> grid(K, M);

    initGrid(grid, tau, h);
    initLayer(grid, tau, h);
       
    int my_rank, commsize;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    std::size_t left_border  = M * my_rank / commsize + 1;
    if(left_border == 0) {
        ++left_border;
    }
    std::size_t right_border = M * (my_rank + 1) / commsize + 1;
    right_border = std::min(right_border, M);

    for(std::size_t i = 1; i < K - 1; ++i) {
        for(std::size_t j = left_border; (j < right_border) && (j < M - 1); ++j) {
            krest(grid, K - 1 - i, j, tau, h);
        }  
        if(my_rank == commsize - 1) {
            angle(grid, K - 1 - i, M - 1, tau, h);
        }

        if(!(my_rank % 2)) {
            if(my_rank != commsize - 1) {
                MPI_Send(&grid[K - 2 - i][right_border - 1], 1, MPI_DOUBLE, my_rank + 1, 0, MPI_COMM_WORLD);
            }
            if(my_rank != 0) {
                MPI_Send(&grid[K - 2 - i][left_border], 1, MPI_DOUBLE, my_rank - 1, 0, MPI_COMM_WORLD);
                MPI_Recv(&grid[K - 2 - i][left_border - 1], 1, MPI_DOUBLE, my_rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
            if(my_rank != commsize - 1) {
                MPI_Recv(&grid[K - 2 - i][right_border], 1, MPI_DOUBLE, my_rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
        } 
        else {
            if(my_rank != 0) {
                MPI_Recv(&grid[K - 2 - i][left_border - 1], 1, MPI_DOUBLE, my_rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
            if(my_rank != commsize - 1) {
                MPI_Recv(&grid[K - 2 - i][right_border], 1, MPI_DOUBLE, my_rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Send(&grid[K - 2 - i][right_border - 1], 1, MPI_DOUBLE, my_rank + 1, 0, MPI_COMM_WORLD);
            }
            if(my_rank != 0) {
                MPI_Send(&grid[K - 2 - i][left_border], 1, MPI_DOUBLE, my_rank - 1, 0, MPI_COMM_WORLD);
            }
        }
    }
    if(my_rank) {
        for(std::size_t i = 2; i < K; ++i) {
            for(std::size_t j = left_border; j < right_border; ++j) {
               MPI_Send(&grid[K - 1 - i][j], 1, MPI_LONG_DOUBLE, 0, 0, MPI_COMM_WORLD);
            }
        }
    } else {
        for(std::size_t rank = 1; rank < commsize; ++rank) {
            left_border  = M * rank / commsize + 1;
            if(left_border == 0) {
                ++left_border;
            }
            right_border = M * (rank + 1) / commsize + 1;
            right_border = std::min(right_border, M);

            for(std::size_t i = 2; i < K; ++i) {
                for(std::size_t j = left_border; j < right_border; ++j) {
                    MPI_Recv(&grid[K - 1 - i][j], 1, MPI_LONG_DOUBLE, rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                }
            }
            
        }
        std::cout << grid << std::endl;
    }
    
    MPI_Finalize();
}
