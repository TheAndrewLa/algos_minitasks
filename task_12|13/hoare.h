#pragma once
#include <vector>
#include <cassert>
#include <cstdlib>
#include <cstdint>
#include <ctime>

#include <iostream>

using usize = std::size_t;
using isize = std::ptrdiff_t;

using std::vector;
using std::pair;
using iterator = std::vector<int>::iterator;

pair<isize, isize> hoare(vector<int>& vec, isize start, isize end) {
    int pivot = vec[start + (std::rand() % (end - start + 1))];

    isize right = end;
    isize left = start;

    isize i = start;

    while (i <= right) {
        if (vec[i] < pivot) {
            std::swap(vec[left], vec[i]);
            i++;
            left++;
        }
        else if (vec[i] == pivot) {
            i++;
        }
        else {
            std::swap(vec[right], vec[i]);
            right--;
        }
    }

    return {left, right};
}

void quicksort_hoare_base(vector<int>& vec, isize start, isize end) {
    if (end - start <= 0) return;

    auto [l, h] = hoare(vec, start, end);
    quicksort_hoare_base(vec, start, l - 1);
    quicksort_hoare_base(vec, h + 1, end);
}

void quicksort_hoare(vector<int>& vec) {
    std::srand(std::time(nullptr));
    quicksort_hoare_base(vec, 0, vec.size() - 1);
}
