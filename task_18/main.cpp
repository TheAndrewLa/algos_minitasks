#include "parser.h"
#include <iostream>

int main(int, char**) {
    MathParser parser {"(1 | 2) & 3"};
    std::cout << parser.to_polish_notation();

    MathParser parser1 {"1 + 2 * 3 + 9 / 3"};
    std::cout << parser1.to_polish_notation();

    MathParser parser2 {"1 + 2 + 3"};
    std::cout << parser2.to_polish_notation();
    
    MathParser parser3 {"1 * (2 / 3)"};
    std::cout << parser3.to_polish_notation();

    MathParser parser4 {"1 * (~2) - 3"};
    std::cout << parser4.to_polish_notation();

    return 0;
}
