#include "binomial_heap.hpp"
#include <sstream>
#include <iostream>

int main(int argc, char** argv) {
    BinomialHeap<std::string> heap{"Hello World", 1};

    heap.insert(12, "Goodbye world!");
    heap.insert(2, "Goodbye world!");
    heap.insert(22, "i");
    heap.insert(32, "Love");
    heap.insert(-2, "C++");

    std::cout << heap.peek_top();

    return 0;
}
