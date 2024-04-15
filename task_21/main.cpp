#include "binomial_heap.hpp"
#include <sstream>
#include <iostream>

int main(int argc, char** argv) {
    BinomialHeap<std::string, int, std::less<int>> heap{"Hello World", 1};
    std::cout << heap.peek_top();

    return 0;
}
