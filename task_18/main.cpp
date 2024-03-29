#include "parser.h"
#include <iostream>

int main(int, char**) {
    MathParser parser1 {"1 * (2 + 1) * 4"};
    std::cout << parser1.to_polish_notation();

    MathParser parser2 {"1 * 2 + 1 * 4"};
    std::cout << parser2.to_polish_notation();

    MathParser parser3 {"3 & [1 | 2]"};
    std::cout << parser3.to_polish_notation();

    MathParser parser4 {"1 * ~2 - 3"};
    std::cout << parser4.to_polish_notation();

    MathParser parser5 {"({[1 + 2 - 1]})"};
    std::cout << parser5.to_polish_notation();

    MathParser parser6 {"-(1 + 4)"};
    std::cout << parser6.to_polish_notation();

    MathParser parser7 {"10 + 3 ** 4 ** 5"};
    std::cout << parser7.to_polish_notation();

    return 0;
}
