#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <cstddef>
#include <ctime>

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

int** mat_mul(int** a, int** b, usize n, usize mod) {
    int** res = create_mat(n);

    for (usize i = 0; i < n; i++)
        for (usize j = 0; j < n; j++)
            for (usize k = 0; k < n; k++)
                res[i][j] += a[i][k] * b[k][j];

    return res;
}

int* mat_vec(int** a, int* b, usize n, usize mod) {
    int* res = create_vec(n);

    for (usize i = 0; i < n; i++)
        for (usize j = 0; j < n; j++)
            res[i] = (res[i] + ((a[i][j] * b[j]) % mod)) % mod;

    return res;
}

// Passing size of matrix as an command line argument
int main(int argc, char ** argv) {
    assert(argc == 2);

    usize size = (usize) std::stoull(string {argv[1]});

    int** a = create_mat(size);

    // Reading matrices from stdin

    for (usize i = 0; i < size; i++)
        for (usize j = 0; j < size; j++)
            std::cin >> a[i][j];

    // Reading vec from stding

    int* b = create_vec(size);

    for (usize i = 0; i < size; i++)
        std::cin >> b[i];

    // for (usize i = 0; i < size; i++)
    //     for (usize j = 0; j < size; j++)
    //         std::cin >> b[i][j];

    // std::clock_t start = std::clock();

    int* res = mat_vec(a, b, size, 7);

    // std::cout << std::clock() - start;

    for (usize i = 0; i < size; i++) {
        //for (usize j = 0; j < size; j++) {
            std::cout << res[i] << ' ';
        //}

        //std::cout << '\n';
    }

    // delete_mat(res, size);
    return 0;
}
