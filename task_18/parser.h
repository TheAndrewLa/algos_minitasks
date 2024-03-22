#pragma once
#include <string>
#include <sstream>

#include <vector>
#include <unordered_map>
#include <tuple>

#include <stdexcept>

#include <cstdlib>

using u8 = std::uint8_t;
using i8 = std::int8_t;

using u16 = std::uint16_t;
using i16 = std::int16_t;

using u32 = std::uint32_t;
using i32 = std::int32_t;

using usize = std::size_t;
using isize = std::ptrdiff_t;

class MathParser {
    public:
    MathParser() = delete;

    explicit MathParser(const std::string& expression);

    MathParser(const MathParser& ref) = delete;
    MathParser(MathParser&& ref) = delete;

    ~MathParser() = default;

    i16 calculate() const noexcept;
    std::string to_polish_notation() const noexcept;

    private:
    using operator_type = u16;
    using operand_type = u16;
    using control_type = u16;

    enum class UnitTypes : u16 {
        OPERATOR, OPERAND, CONTROL
    };

    // WARNING: SYSTEM PROGRAMMING STUFF!!!

    // Bytecode explanation 

    // Operator type takes 2 bytes
    // Using this method we can handle 16 operators (enough for simple arithmetic parser)

    // Format of operator:
    // | operator_code | - | priority | - | associativity | - |  arity  |
    // |     4 bits    | - |  4 bits  | - |    2 bits     | - |  2 bits |

    // Control type (it just brackets) takes also 2 bytes

    // Format of control:
    // |   order (closing/opening)  | - |   type (round, box, curve)    |
    // |         8 bits             | - |         8 bits                |

    // Operand type takes also 2 bytes (it's just a signed 16-bit number)

    // Format of operand
    // | actual_value |
    // |   16 bits    |

    // Operators
    static constexpr char* operators[] {
        "+", "-", "*", "/", "%",
        "&", "^", "|", "~",
        "<<", ">>",
    };

    static constexpr u16 priority[] {
        2, 2, 1, 1, 1,
        4, 5, 6, 0,
        3, 3,
    };

    static constexpr u16 associativities[] {
        0, 0, 0, 0, 0,
        0, 0, 0, 1,
        0, 0,
    };

    static constexpr u16 arity[] {
        1, 1, 1, 1, 1,
        1, 1, 1, 0,
        1, 1,
    };

    // Brackets
    static constexpr char* brackets[] {
        "(", "[", "{",
        ")", "]", "}",
    };

    static constexpr u16 order[] {
        0, 0, 0,
        1, 1, 1,
    };

    // Every bytecode command has 4 bytes size
    struct SyntaxUnit {
        UnitTypes type;

        union {
            operator_type oprt;
            operand_type oprd;
            control_type ctrl;
        };
    };

    struct WritingUnit {
        std::string str;
        usize priority;
    };

    struct WritingUnitComparator {
        constexpr bool operator() (const WritingUnit& left, const WritingUnit& right) {
            return (left.priority > right.priority);
        }
    };

    std::vector<SyntaxUnit> syntax_;
};
