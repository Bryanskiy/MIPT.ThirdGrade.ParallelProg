#include <vector>
#include <iostream>
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
    std::size_t K, M; std::cin >> K >> M;
    gtype tau, h; std::cin >> tau >> h;

    matrix::matrix_t<gtype> grid(K, M);

    initGrid(grid, tau, h);
    initLayer(grid, tau, h);
        
    for(std::size_t i = 1; i < K - 1; ++i) {
        for(std::size_t j = 1; j < M - 1; ++j) {
            krest(grid, K - 1 - i, j, tau, h);
        }
        angle(grid, K - 1 - i, M - 1, tau, h);
    }
    
    std::cout << grid << std::endl;
}
