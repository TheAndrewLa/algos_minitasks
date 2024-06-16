#pragma once
#include <vector>
#include <cassert>
#include <cstdlib>
#include <cstdint>
#include <ctime>

using isize = std::ptrdiff_t;

using std::vector;
using std::pair;

pair<isize, isize> lomuto(vector<int>& vec, isize start, isize end) {
    isize pivot_index = start + (std::rand() % (end - start + 1));
    int pivot = vec[pivot_index];

    std::swap(vec[start], vec[pivot_index]);

    isize lo = start;
    isize hi = start;

    for (auto i = start + 1; i <= end; i++) {
        if (vec[i] < pivot) {
            // Chain swapping (my own naming btw)

            int tmp = vec[i];
            vec[i] = vec[hi + 1];
            vec[hi + 1] = vec[lo];
            vec[lo] = tmp;

            lo++;
            hi++;
        }
        else if (vec[i] == pivot) {
            std::swap(vec[hi + 1], vec[i]);
            hi++;
        }
    }

    return {lo, hi};
}

void quicksort_lomuto_base(vector<int>& vec, isize start, isize end) {
    if (end - start <= 0) return;

    auto [l, h] = lomuto(vec, start, end);

    quicksort_lomuto_base(vec, start, l - 1);
    quicksort_lomuto_base(vec, h + 1, end);
}

void quicksort_lomuto(vector<int>& vec) {
    std::srand(std::time(nullptr));
    quicksort_lomuto_base(vec, 0, vec.size() - 1);
}
