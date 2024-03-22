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

static bool try_merge(std::string& current, char next) {
    if (std::is_number(current) && std::isdigit(next)) {
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
    std::string symbol_table {" 0123456789+-*/%<>&|~()[]{}"};
    std::vector<std::string> str_tokens;

    for (char i : expression) {
        usize pos = symbol_table.find(i);
        
        if (pos == 0)
            continue;

        else if (pos == std::string::npos)
            throw std::invalid_argument("Unresolved symbol in expression: " + std::string {i});

        str_tokens.push_back(std::string {i});
    }

    for (auto i = str_tokens.begin(); i < str_tokens.end() - 1; i++) {
        auto next = i + 1;

        while (try_merge(*i, next->at(0)))
            str_tokens.erase(next);
    }

    // Convert string to syntax unit structs

    for (const auto& i : str_tokens) {
        bool flag = true;

        if (std::is_number(i)) {
            this->syntax_.push_back({
                .type = UnitTypes::OPERAND,
                .oprd = static_cast<u16>(std::stoi(i))
            });

            flag = false;
        }

        for (auto j = std::begin(operators); j < std::end(operators) && flag; j++) {
            if (i == *j) {
                auto index = static_cast<u16>(std::distance(std::begin(operators), j));

                u16 operation_code = (index & 0xF) << 12;
                u16 operation_priority = (priority[index] & 0xF) << 4;
                u16 operation_associativity = (associativities[index] & 0x3) << 2;
                u16 operation_arity = arity[index] & 0x3;

                u16 operation_mask = operation_code | operation_priority | operation_associativity | operation_arity;

                this->syntax_.push_back({
                    .type = UnitTypes::OPERATOR,
                    .oprt = operation_mask
                });

                flag = false;
            }
        }

        for (auto j = std::begin(brackets); j < std::end(brackets) && flag; j++) {
            if (i == *j) {
                auto index = static_cast<u16>(std::distance(std::begin(brackets), j));
                
                u16 bracket_order = (order[index] << 8) & 0xFF00;
                u16 bracket_type = (index % 3) & 0x00FF;

                u16 bracket_mask = bracket_order | bracket_type;

                this->syntax_.push_back({
                    .type = UnitTypes::CONTROL,
                    .ctrl = bracket_mask
                });

                flag = false;
            }
        }

        if (flag)
            throw std::invalid_argument("Syntax error: " + i);
    }
}

i16 MathParser::calculate() const noexcept {
    i16 result = 0;
    return result;
}

std::string MathParser::to_polish_notation() const noexcept {
    std::ostringstream result;
    return result.str();
}
