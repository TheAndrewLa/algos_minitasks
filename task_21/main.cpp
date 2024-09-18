#include "binomial_heap.hpp"
#include <sstream>
#include <iostream>

int main(int argc, char** argv) {
    BinomialHeap<std::string, int, std::less<int>> heap{"Hello World", 1};

    heap.insert("Goodbye world!", 12);
    heap.insert("i", 22);
    heap.insert("And", 23);
    heap.insert("hate", 1);
    heap.insert("Python", 2);
    heap.insert("Love", 32);

    std::cout << heap.peek_top() << '\n';
    std::cout << heap.peek_top() << '\n';
    std::cout << heap.extract_top() << '\n';

    return 0;
}
