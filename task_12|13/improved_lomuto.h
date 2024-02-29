#pragma once
#include <assert.h>
#include <stdlib.h>

extern "C" {
    typedef size_t usize;
    typedef intptr_t isize;

    void swap(const void* a, const void* b) {
        const void* tmp = a;
        a = b;
        b = tmp;
    }

    int* partition(int* start, int* end) {
        isize len = end - start;
        if (len <= 1)
            return start;

        end--;

        if (*start > *end)
            swap(start, end);
        
        int pivot = *start;
        int* pivot_ptr = start;

        do {
            start++;
        } while (*start < pivot);

        for (int* c = start + 1; c < end; c++) {
            int value = *c;
            int less = (int) (value < pivot);
            int delta = less & (c - start);

            start[delta] = *start;
            c[-delta] = value;
            start -= less;
        }

        start--;
        *pivot_ptr = *start;
        *start = pivot;

        return start;
    }
}
