#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <cstddef>
#include <chrono>

using usize = std::size_t;
using std::string;

int** create_mat(usize n) {
    int** res = new int*[n];
    for (usize i = 0; i < n; i++)
        res[i] = new int[n]();

    return res;
}

int* create_vec(usize n) {
    int* res = new int[n]();
    return res;
}

int delete_mat(int** mat, usize n) {
    for (usize i = 0; i < n; i++)
        delete[] mat[i];

    delete[] mat;

    return 0;
}

// Usual algorithm: O(n^3), iteration

int** mat_mul(int** a, int** b, usize n) {
    int** res = create_mat(n);

    for (usize i = 0; i < n; i++)
        for (usize j = 0; j < n; j++)
            for (usize k = 0; k < n; k++)
                res[i][j] += a[i][k] * b[k][j];

    return res;
}

// Passing size of matrix as an command line argument
int main(int argc, char ** argv) {
    usize size;
    std::cin >> size;

    int** a = create_mat(size);
    int** b = create_mat(size);

    for (usize i = 0; i < size; i++)
        for (usize j = 0; j < size; j++)
            std::cin >> a[i][j];

    for (usize i = 0; i < size; i++)
        for (usize j = 0; j < size; j++)
            std::cin >> b[i][j];

    auto start_time = clock();
    int** res = mat_mul(a, b, size);
    auto elapsed = clock() - start_time;

    std::cout << elapsed;

    return delete_mat(res, size);
}
