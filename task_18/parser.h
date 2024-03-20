#pragma once
#include <string>

#include <vector>
#include <stack>
#include <queue>

#include <stdexcept>

using usize = std::size_t;
using isize = std::ptrdiff_t;

class MathParser {
    public:
    MathParser() = delete;

    explicit MathParser(const std::string& expression);

    MathParser(const MathParser& ref) = delete;
    MathParser(MathParser&& ref) = delete;

    ~MathParser() = default;

    int calculate() const noexcept;
    std::string polish_notation() const noexcept;

    private:
    enum class operators : usize {
        INC, DEC,
        ADD, SUB, MUL, DIV, REM,
        AND, XOR, OR,
        SLA, SRA,
    };

    enum class brackets {
        ROUND_OPEN, ROUND_CLOSE,
        BOX_OPEN, BOX_CLOSE,
    };

    struct operation {
        usize op_index;
        int* args;
    };

    struct operation_comparator {
        constexpr bool operator()(const operation& left, const operation& right) const {
            return MathParser::priorities_[left.op_index] > MathParser::priorities_[right.op_index];
        }
    };

    static constexpr isize priorities_[] {
        0, 0,
        2, 2, 1, 1, 1,
        3, 4, 6,
        3, 3,
    };

    static const isize priority_bias_ = 6;

    static constexpr usize arities_[] {
        1, 1,
        2, 2, 2, 2, 2,
        2, 2, 2,
        2, 2,
    };

    std::priority_queue<operation, std::vector<operation>, operation_comparator> operation_queue_;
};
