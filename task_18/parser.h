#pragma once
#include <string>
#include <sstream>

#include <vector>
#include <stack>

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

    enum class SyntaxTypes : u16 {
        OPERATOR, OPERAND, CONTROL
    };

    // WARNING: SYSTEM PROGRAMMING STUFF!!!
    // EXPLANATION OF MY BYTECODE

    // Operator type takes 2 bytes
    // Using this method we can handle 16 operators (enough for simple arithmetic parser)

    // Format of operator:
    // | operator_code | - | priority | - | associativity | - |  arity  |
    // |     4 bits    | - |  4 bits  | - |    4 bits     | - |  4 bits |

    // Control type (just brackets) takes also 2 bytes

    // Format of control:
    // |   type (round, box, curve)  | - |   order (closing/opening)     |
    // |         8 bits              | - |         8 bits                |

    // Operand type takes also 2 bytes (it's just a signed 16-bit number)

    // Format of operand
    // | number_value |
    // |   16 bits    |

    // Operators
    static const usize operators_count = 12;

    static constexpr const char* operators[] {
        "+", "-", "*", "/", "%",
        "&", "^", "|", "~",
        "**",
        "<<", ">>",
    };

    // TODO: combine it into one array (magic with bits)
    static constexpr u16 priority[] {
        4, 4, 3, 3, 3,
        6, 7, 8, 0,
        2,
        5, 5,
    };

    static constexpr u16 operator_masks[] {};

    static constexpr u16 associativities[] {
        0, 0, 0, 0, 0,
        0, 0, 0, 1,
        1,
        0, 0,
    };

    static constexpr u16 arity[] {
        2, 2, 2, 2, 2,
        2, 2, 2, 1,
        2,
        2, 2,
    };

    // Brackets
    static const usize brackets_count = 6;

    static constexpr const char* brackets[] {
        "(", "[", "{",
        ")", "]", "}",
    };

    // TODO: compine it into one array (magic with bits)
    static constexpr u16 type[] {
        0, 1, 2,
        0, 1, 2,
    };

    static constexpr u16 order[] {
        0, 0, 0,
        1, 1, 1,
    };

    // Every bytecode command has 4 bytes size
    struct SyntaxUnit {
        SyntaxTypes type;

        union {
            operator_type oprt;
            operand_type oprd;
            control_type ctrl;
        };

        SyntaxUnit() = delete;
        SyntaxUnit(const SyntaxUnit& unit) = default;
        SyntaxUnit(SyntaxUnit&& unit) = default;

        ~SyntaxUnit() = default;

        std::string to_string() const noexcept {
            switch (this->type) {
                case (SyntaxTypes::OPERATOR): {
                    return {MathParser::operators[static_cast<usize>(this->oprd >> 0xC)]};
                }

                case (SyntaxTypes::OPERAND): {
                    return std::to_string(static_cast<i32>(this->oprt));
                }

                case (SyntaxTypes::CONTROL): {
                    auto index = static_cast<usize>(this->ctrl >> 0x8) + 3 * static_cast<usize>(this->ctrl & 0xF);
                    return {MathParser::brackets[index]};
                }

                default: {
                    return "ERROR!";
                }
            }
        }
    };

    std::vector<SyntaxUnit> syntax_;
};
