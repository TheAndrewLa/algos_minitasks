#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <cstddef>
#include <chrono>

using usize = std::size_t;
using isize = std::ptrdiff_t;
using std::string;

#define MATRIX_OFFSET(mat, size, y_offset, x_offset) (mat + ((size) * (y_offset)) + (x_offset))

int** create_mat(usize n) {
    int** res = new int*[n];
    for (usize i = 0; i < n; i++)
        res[i] = new int[n];

    return res;
}

int delete_mat(int** mat, usize n) {
    for (usize i = 0; i < n; i++)
        delete[] mat[i];

    delete[] mat;

    return 0;
}

int** mat_add_assign(int** a, int** b, usize size) {
    assert(a != nullptr && b != nullptr);

    for (usize i = 0; i < size; i++)
        for (usize j = 0; j < size; j++)
            a[i][j] += b[i][j];

    return a;
}

int** mat_sub_assign(int** a, int** b, usize size) {
    assert(a != nullptr && b != nullptr);

    for (usize i = 0; i < size; i++)
        for (usize j = 0; j < size; j++)
            a[i][j] -= b[i][j];

    return a;
}

int** mat_add(int** a, int** b, usize size) {
    assert(a != nullptr && b != nullptr);

    int** res = create_mat(size);

    for (usize i = 0; i < size; i++)
        for (usize j = 0; j < size; j++)
            res[i][j] = a[i][j] + b[i][j];

    return res;
}

int** mat_sub(int** a, int** b, usize size) {
    assert(a != nullptr && b != nullptr);

    int** res = create_mat(size);

    for (usize i = 0; i < size; i++)
        for (usize j = 0; j < size; j++)
            res[i][j] = a[i][j] - b[i][j];

    return res;
}

inline void zero_mat(int** mat, usize n) {
    mat_sub_assign(mat, mat, n);
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

    if (n <= 64)
        return mat_mul(in_a, in_b, n);

    usize half = n / 2;

    // 8 matrices
    int** a = create_mat(half);
    int** b = create_mat(half);
    int** c = create_mat(half);
    int** d = create_mat(half);

    int** e = create_mat(half);
    int** f = create_mat(half);
    int** g = create_mat(half);
    int** h = create_mat(half);

    for (usize i = 0; i < half; i++) {
        for (usize j = 0; j < half; j++) {
            a[i][j] = in_a[i][j];
            b[i][j] = in_a[i][j + half];
            c[i][j] = in_a[i + half][j];
            d[i][j] = in_a[i + half][j + half];

            e[i][j] = in_b[i][j];
            f[i][j] = in_b[i][j + half];
            g[i][j] = in_b[i + half][j];
            h[i][j] = in_b[i + half][j + half];
        }
    }

    int** tmp_mat = create_mat(half);
    int** tmp_mat_2 = create_mat(half);

    zero_mat(tmp_mat, half);
    zero_mat(tmp_mat_2, half);

    tmp_mat = mat_add_assign(tmp_mat, f, half);
    tmp_mat = mat_sub_assign(tmp_mat, h, half);

    int** p1 = mat_mul_recursive(a, tmp_mat, half);

    zero_mat(tmp_mat, half);

    tmp_mat = mat_add_assign(tmp_mat, a, half);
    tmp_mat = mat_add_assign(tmp_mat, b, half);

    int** p2 = mat_mul_recursive(tmp_mat, h, half);

    zero_mat(tmp_mat, half);

    tmp_mat = mat_add_assign(tmp_mat, c, half);
    tmp_mat = mat_add_assign(tmp_mat, d, half);

    int** p3 = mat_mul_recursive(tmp_mat, e, half);

    zero_mat(tmp_mat, half);

    tmp_mat = mat_add_assign(tmp_mat, g, half);
    tmp_mat = mat_add_assign(tmp_mat, e, half);

    int** p4 = mat_mul_recursive(d, tmp_mat, half);
    
    zero_mat(tmp_mat, half);
    zero_mat(tmp_mat_2, half);

    tmp_mat = mat_add_assign(tmp_mat, a, half);
    tmp_mat = mat_add_assign(tmp_mat, d, half);

    tmp_mat_2 = mat_add_assign(tmp_mat_2, e, half);
    tmp_mat_2 = mat_add_assign(tmp_mat_2, h, half);

    int** p5 = mat_mul_recursive(tmp_mat, tmp_mat_2, half);

    zero_mat(tmp_mat, half);
    zero_mat(tmp_mat_2, half);

    tmp_mat = mat_add_assign(tmp_mat, b, half);
    tmp_mat = mat_sub_assign(tmp_mat, d, half);

    tmp_mat_2 = mat_add_assign(tmp_mat_2, g, half);
    tmp_mat_2 = mat_add_assign(tmp_mat_2, h, half);

    int** p6 = mat_mul_recursive(tmp_mat, tmp_mat_2, half);

    zero_mat(tmp_mat, half);
    zero_mat(tmp_mat_2, half);

    tmp_mat = mat_add_assign(tmp_mat, a, half);
    tmp_mat = mat_sub_assign(tmp_mat, c, half);

    tmp_mat_2 = mat_add_assign(tmp_mat_2, e, half);
    tmp_mat_2 = mat_add_assign(tmp_mat_2, g, half);

    int** p7 = mat_mul_recursive(tmp_mat, tmp_mat_2, half);

    int** q1 = mat_add(p5, p4, half);
    q1 = mat_sub_assign(q1, p2, half);
    q1 = mat_add_assign(q1, p6, half);

    int** q2 = mat_add(p1, p2, half);
    int** q3 = mat_add(p3, p4, half);

    int** q4 = mat_add(p1, p5, half);
    q4 = mat_sub_assign(q4, p3, half);
    q4 = mat_sub_assign(q4, p7, half);

    delete_mat(p1, half);
    delete_mat(p2, half);
    delete_mat(p3, half);
    delete_mat(p4, half);
    delete_mat(p5, half);
    delete_mat(p6, half);
    delete_mat(p7, half);

    int** res = create_mat(n);

    for (usize i = 0; i < half; i++) {
        for (usize j = 0; j < half; j++) {
            res[i][j] = q1[i][j];
            res[i][j + half] = q2[i][j];
            res[i + half][j] = q3[i][j];
            res[i + half][j + half] = q4[i][j];
        }
    }

    delete_mat(q1, half);
    delete_mat(q2, half);
    delete_mat(q3, half);
    delete_mat(q4, half);

    return res;
}

// Passing size of matrix as an command line argument
int main(int argc, char** argv) {
    usize size;
    std::cin >> size;

    int** a = create_mat(size);
    int** b = create_mat(size);

    // Reading matrices from stdin

    for (usize i = 0; i < size; i++)
        for (usize j = 0; j < size; j++)
            std::cin >> a[i][j];

    for (usize i = 0; i < size; i++)
        for (usize j = 0; j < size; j++)
            std::cin >> b[i][j];

    auto start_time = clock();
    int** res = mat_mul_recursive(a, b, size);
    auto elapsed = clock() - start_time;

    std::cout << elapsed;

    return delete_mat(res, size);
}
