#pragma once
#include <vector>
#include <cassert>
#include <cstdlib>

using usize = std::size_t;
using isize = std::ptrdiff_t;

using std::vector;
using std::pair;
using iterator = std::vector<int>::iterator;

pair<iterator, iterator> lomuto(iterator start, iterator end) {
    iterator pivot = start + (std::rand() % std::distance(start, end));
    std::swap(*start, *pivot);

    iterator lo = start;
    iterator hi = start;

    for (auto i = start + 1; i < end; i++) {
        if (*i < *pivot) {
            // Chain swapping
            int tmp = *i;
            *i = *(hi + 1);
            *(hi + 1) = *lo;
            *lo = tmp;

            lo++;
            hi++;
        }
        else if (*i == *pivot) {
            std::swap(*(hi + 1), *i);
            hi++;
        }
    }

    return {lo, hi};
}

void quicksort_lomuto(iterator begin, iterator end) {
    std::srand(std::time(nullptr));

    if (std::distance(begin, end) <= 1)
        return;

    auto [l, h] = lomuto(begin, end);
    quicksort_lomuto(begin, l);
    quicksort_lomuto(h + 1, end);
}
