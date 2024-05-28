#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <cstddef>

using usize = std::size_t;
using isize = std::ptrdiff_t;
using std::string;

#define MATRIX_OFFSET(mat, size, x_offset, y_offset) (int**) ((mat) + (sizeof(int) * (size) * (x_offset)) + (sizeof(int) * (y_offset)))

int** create_mat(usize n) {
    int** res = new int*[n];
    for (usize i = 0; i < n; i++)
        res[i] = new int[n]();

    return res;
}

int delete_mat(int** mat, usize n) {
    for (usize i = 0; i < n; i++)
        delete[] mat[i];

    delete[] mat;

    return 0;
}

// Resursive algorithm. Also O(n^3)

const usize RecursionFallback = 16;

void mat_add(int** a, int** b, usize size) {
    assert(a != nullptr && b != nullptr);

    for (usize i = 0; i < size; i++)
        for (usize j = 0; j < size; j++)
            a[i][j] += b[i][j];
}

int** mat_mul(int** a, int** b, usize n) {
    assert(a != nullptr && b != nullptr);

    int** res = create_mat(n);

    for (usize i = 0; i < n; i++)
        for (usize j = 0; j < n; j++)
            for (usize k = 0; k < n; k++)
                res[i][j] += a[i][k] * b[k][j];

    return res;
}

int** mat_mul_recursive(int** in_a, int** in_b, usize n) {
    assert(in_a != nullptr && in_b != nullptr);

    if (n <= RecursionFallback)
        return mat_mul(in_a, in_b, n);

    usize half = n / 2;

    // 8 matrices
    int** a = MATRIX_OFFSET(in_a, n, 0, 0);
    int** b = MATRIX_OFFSET(in_a, n, 0, half);
    int** c = MATRIX_OFFSET(in_a, n, half, 0);
    int** d = MATRIX_OFFSET(in_a, n, half, half);

    int** e = MATRIX_OFFSET(in_b, n, 0, 0);
    int** f = MATRIX_OFFSET(in_b, n, 0, half);
    int** g = MATRIX_OFFSET(in_b, n, half, 0);
    int** h = MATRIX_OFFSET(in_b, n, half, half);

    int** ae = mat_mul_recursive(a, e, half);
    int** bg = mat_mul_recursive(b, g, half);
    mat_add(ae, bg, half);
    delete_mat(bg, half);

    int** q1 = ae;

    int** af = mat_mul_recursive(a, f, half);
    int** bh = mat_mul_recursive(b, h, half);
    mat_add(af, bh, half);
    delete_mat(bh, half);

    int** q2 = af;

    int** ce = mat_mul_recursive(c, e, half);
    int** dg = mat_mul_recursive(d, g, half);
    mat_add(ce, dg, half);
    delete_mat(dg, half);

    int** q3 = ce;

    int** cf = mat_mul_recursive(c, f, half);
    int** dh = mat_mul_recursive(d, h, half);
    mat_add(cf, dh, half);
    delete_mat(dh, half);

    int** q4 = cf;

    int** res = create_mat(n);

    // Perform copying
    for (usize i = 0; i < half; i++)
        for (usize j = 0; j < half; j++)
            res[i][j] = q1[i][j];

    for (usize i = 0; i < half; i++)
        for (usize j = half; j < n; j++)
            res[i][j] = q2[i][j - half];

    for (usize i = half; i < n; i++)
        for (usize j = 0; j < half; j++)
            res[i][j] = q3[i - half][j];

    for (usize i = half; i < n; i++)
        for (usize j = half; j < n; j++)
            res[i][j] = q4[i - half][j - half];

    delete_mat(q1, half);
    delete_mat(q2, half);
    delete_mat(q3, half);
    delete_mat(q4, half);

    return res;
}

// Passing size of matrix as an command line argument
int main(int argc, char ** argv) {
    assert(argc == 2);

    usize size = (usize) std::stoull(string {argv[1]});

    int** a = create_mat(size);
    int** b = create_mat(size);

    // Reading matrices from stdin

    for (usize i = 0; i < size; i++)
        for (usize j = 0; j < size; j++)
            std::cin >> a[i][j];

    for (usize i = 0; i < size; i++)
        for (usize j = 0; j < size; j++)
            std::cin >> b[i][j];

    std::clock_t start = std::clock();
    int** res = mat_mul(a, b, size);
    std::cout << std::clock() - start;

    return delete_mat(res, size);
}
