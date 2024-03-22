#include "parser.h"

int main(int, char**) {
    MathParser parser {"22 + (22 * 22) - [5 / 4] + {45}"};
    parser.optimize_bytecode();

    return 0;
}
