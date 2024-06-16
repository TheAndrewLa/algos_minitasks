#pragma once
#include <assert.h>
#include <stdlib.h>

using usize = size_t;
using isize = ptrdiff_t;

using std::swap;

template <typename It>
It lomuto_partition_branchfree(It first, It last) {
    assert(first <= last);
    if (last - first < 2)
        return first;

    --last;
    if (*first > *last)
        swap(*first, *last);

    auto pivot_pos = first;
    auto pivot = *first;

    do {
        ++first;
        assert(first <= last);
    } while (*first < pivot);

    for (auto read = first + 1; read < last; ++read) {
        auto x = *read;
        auto less = -int(x < pivot);
        auto delta = less & (read - first);
        first[delta] = *first;
        read[-delta] = x;
        first -= less;
    }
    // Move the pivot to its final slot.
    assert(*first >= pivot);
    --first;
    *pivot_pos = *first;
    *first = pivot;
    return first;
}

template <typename It>
void unguarded_linear_insert(It last) {
    auto val = *last;
    --last;
    auto x = *last;
    if (val >= x)
        return;
    for (;;) {
        last[1] = x;
        --last;
        x = *last;
        if (val >= x)
            break;
    }
    last[1] = val;
}

template <typename It>
void insertion_sort(It first, It last) {
    assert(first <= last);
    for (auto i = first + 1; i < last; ++i) {
        auto val = *i;
	    if (val < *first) {
            size_t n = i - first - 1;
            do {
                first[n + 1] = first[n];
            }
            while (n--);
	        *first = val;
	    }
	    else {
	        unguarded_linear_insert(i);
        }
	}
}

template <typename It>
void sort(It first, It last) {
    while (last - first > 16) {
	    auto cut = lomuto_partition_branchfree(first, last);

        assert(cut >= first);
        assert(cut < last);
	    sort(cut + 1, last);
	    last = cut;
	}

    insertion_sort(first, last);
}

void quicksort_new_lomuto(vector<int>& vec) {
    ::sort(vec.begin().base(), vec.end().base());
}
