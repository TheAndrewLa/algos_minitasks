#pragma once
#include <vector>
#include <cassert>
#include <cstdlib>

using usize = std::size_t;
using isize = std::ptrdiff_t;

using std::vector;
using std::pair;
using iterator = std::vector<int>::iterator;

pair<iterator, iterator> hoare(iterator start, iterator end) {
    iterator pivot = start + (std::rand() % std::distance(start, end));

    iterator i = start - 1;
    iterator j = end;

    while (true) {
        do {
            i++;
        } while (*i < *pivot);

        do {
            j--;
        } while (*j > *pivot);

        if (i >= j)
            return {i, j};

        std::swap(*i, *j);
    }

    return {i, j};
}

void quicksort_hoare(iterator begin, iterator end) {
    std::srand(std::time(nullptr));
    if (std::distance(begin, end) <= 1)
        return;

    auto [l, h] = hoare(begin, end);
    quicksort_hoare(begin, l);
    quicksort_hoare(h + 1, end);
}
