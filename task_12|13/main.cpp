#include "hoare.h"
#include "lomuto.h"
#include "improved_lomuto.h"
#include <iostream>
#include <chrono>

const usize TEST_SIZE = 1000;

void lomuto_test(vector<int>& vec) {
    const auto start = std::chrono::steady_clock::now();
    quicksort_lomuto(vec);
    const auto end = std::chrono::steady_clock::now();

    const auto diff = end - start;
    std::cout << "Lomuto takes: " << diff.count() << '\n';
}

void hoare_test(vector<int>& vec) {
    const auto start = std::chrono::steady_clock::now();
    quicksort_hoare(vec);
    const auto end = std::chrono::steady_clock::now();

    const auto diff = end - start;
    std::cout << "Hoare takes: " << diff.count() << '\n';
}

void weird_lomuto_test(vector<int>& vec) {
    const auto start = std::chrono::steady_clock::now();
    quicksort_new_lomuto(vec);
    const auto end = std::chrono::steady_clock::now();

    const auto diff = end - start;
    std::cout << "Weird lomuto takes: " << diff.count() << '\n';
}

int main(int argc, char** argv) {
    std::vector<int> vec1(TEST_SIZE, 0);
    std::vector<int> vec2(TEST_SIZE, 0);
    std::vector<int> vec3(TEST_SIZE, 0);

    for (usize i = 0; i < TEST_SIZE; i++) {
        int val = std::rand() % 100;
        vec1[i] = val;
        vec2[i] = val;
        vec3[i] = val;
    }

    lomuto_test(vec1);
    hoare_test(vec2);
    weird_lomuto_test(vec3);
}
