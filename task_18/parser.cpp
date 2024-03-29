#include "parser.h"
#include <iostream>

namespace std {
    bool is_number(const std::string& str) {
        if (str.empty())
            return false;

        for (const auto& i : str) {
            if (!isdigit(i))
                return false;
        }

        return true;
    }

    bool is_1sym_operation(const std::string& str) {
        return (str == "+" || str == "-" || str == "*" || str == "/" || str == "%" || str == "~" || str == "&" || str == "|");
    }

    bool is_2sym_operation(const std::string& str) {
        return (str == "<<" || str == ">>");
    }

    bool is_operator(const std::string& str) {
        return (is_1sym_operation(str) || is_2sym_operation(str));
    }

    bool is_bracket(const std::string& str) {
        return (str == "(" || str == ")" || str == "[" || str == "]" || str == "{" || str == "}");
    }
}

static bool merge_tokens(std::string& current, char next) {
    if (std::is_number(current) && std::isdigit(next)) {
        current += next;
        return true;
    }
    else if (current == "*" && next == '*') {
        current += next;
        return true;
    }
    else if (std::is_1sym_operation(current)) {
        return false;
    }
    else if (current == "<" && next == '<') {
        current += next;
        return true;
    }
    else if (current == ">" && next == '>') {
        current += next;
        return true;
    }
    else if (std::is_bracket(current)) {
        return false;
    }

    return false;
}

MathParser::MathParser(const std::string& expression) {
    std::string symbol_table {" 0123456789+-*/%<>&|^~()[]{}"};
    std::vector<std::string> str_tokens;

    for (char i : expression) {
        usize pos = symbol_table.find(i);
        
        if (pos == 0)
            continue;

        else if (pos == symbol_table.npos)
            throw std::invalid_argument("Unresolved symbol: " + std::string {i});

        str_tokens.push_back(std::string {i});
    }

    for (usize i = 0; i < str_tokens.size() - 1; i++) {
        while (i + 1 < str_tokens.size() && merge_tokens(str_tokens[i], str_tokens[i + 1][0]))
            str_tokens.erase(str_tokens.cbegin() + i + 1);
    }

    // Convert string to syntax unit structs (bytecode itself)

    for (const auto& i : str_tokens) {
        bool flag = true;

        if (std::is_number(i)) {
            this->syntax_.push_back({
                .type = SyntaxTypes::OPERAND,
                .oprd = static_cast<u16>(std::stoi(i)),
            });

            flag = false;
        }

        for (auto j = 0; j < operators_count && flag; j++) {
            if (i == operators[j]) {
                u16 operation_code = j << 0xC;
                u16 operation_priority = priority[j] << 0x8;
                u16 operation_associativity = associativities[j] << 0x4;
                u16 operation_arity = arity[j];
                u16 operation_mask = operation_code | operation_priority | operation_associativity | operation_arity;

                this->syntax_.push_back({
                    .type = SyntaxTypes::OPERATOR,
                    .oprt = operation_mask,
                });

                flag = false;
            }
        }

        for (auto j = 0; j < brackets_count && flag; j++) {
            if (i == brackets[j]) {
                u16 bracket_type = type[j] << 0x8;
                u16 bracket_order = order[j];

                u16 bracket_mask = bracket_order | bracket_type;

                this->syntax_.push_back({
                    .type = SyntaxTypes::CONTROL,
                    .ctrl = bracket_mask,
                });

                flag = false;
            }
        }

        if (flag)
            throw std::invalid_argument("Syntax error: " + i);
    }

    // Just for throwing exceptions (ha-ha)
    // TODO: check operators arity
    // TODO: check brackets
}

// TODO: implement it
i16 MathParser::calculate() const noexcept {
    i16 result = 0;
    return result;
}

std::string MathParser::to_polish_notation() const noexcept {
    std::ostringstream result;
    std::stack<SyntaxUnit, std::vector<SyntaxUnit>> ops;

    for (const auto& i : this->syntax_) {
        if (i.type == SyntaxTypes::OPERAND) {
            result << i.oprd << ' ';
        }

        else if (i.type == SyntaxTypes::CONTROL) {
            u16 bracket_type = i.ctrl >> 0x8;
            u16 bracket_order = i.ctrl & 0xFF;

            if (bracket_order == 0) {
                ops.push(i);
            }
            else {
                u16 mask = i.ctrl | 0x1;

                while (ops.top().type != SyntaxTypes::CONTROL && ops.top().ctrl != mask) {
                    result << ops.top().to_string() << ' ';
                    ops.pop();
                }

                ops.pop();
            }
        }

        else if (i.type == SyntaxTypes::OPERATOR) {
            u16 op_priority = (i.oprt >> 0x8) & 0xF;
            u16 op_associativity = (i.oprt >> 0x4) & 0xF;

            // Take operators from stack which has greater priority than given one

            while (!ops.empty() && ops.top().type == SyntaxTypes::OPERATOR && 
            ((ops.top().oprt >> 0x8) & 0xF) <= op_priority && op_associativity == 0) {
                result << ops.top().to_string() << ' ';
                ops.pop();
            }

            while (!ops.empty() && ops.top().type == SyntaxTypes::OPERATOR &&
            ((ops.top().oprt >> 0x8) & 0xF) < op_priority && op_associativity == 1) {
                result << ops.top().to_string() << ' ';
                ops.pop();
            }

            ops.push(i);
        }
    }

    while (!ops.empty()) {
        result << ops.top().to_string() << ' ';
        ops.pop();
    }

    result << '\n';
    return result.str();
}
