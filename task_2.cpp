#include <iostream>
#include <vector>
#include <string>
#include <tuple>
#include <algorithm>

using std::string;
using std::vector;

// First and second is for input numbers and the third one is for expecting result
using test_unit = std::tuple<string, string, string>;

using ulong = unsigned long;
using uchar = unsigned char;
using usize = std::size_t;

string add(string x, string y) {
    string res;
    ulong memory = 0;

    // Make numbers be same length
    while (x.length() < y.length())
        x = '0' + x;
    
    while (y.length() < x.length())
        y = '0' + y;

    string::reverse_iterator x_iter = x.rbegin();
    string::reverse_iterator y_iter = y.rbegin();

    string::reverse_iterator x_end = x.rend();
    string::reverse_iterator y_end = y.rend();

    while (x_iter < x_end && y_iter < y_end) {
        ulong d1 = (ulong) ((*x_iter) - '0');
        ulong d2 = (ulong) ((*y_iter) - '0');

        ulong sum = d1 + d2 + memory;

        char low = (char) (sum % 10) + '0';
        memory = sum / 10;

        res.push_back(low);

        x_iter++;
        y_iter++;
    }

    if (memory != 0)
        res.push_back((char) (memory) + '0');

    std::reverse(res.begin(), res.end());

    return res;
}

// Subtraction needs Y to be lower than X
string sub(string x, string y) {
    string res;
    ulong memory = 0;

    if (x.length() < y.length()) {
        return "0";
    }

    // Make numbers be same length
    while (x.length() < y.length())
        x = '0' + x;
    
    while (y.length() < x.length())
        y = '0' + y;

    string::reverse_iterator x_iter = x.rbegin();
    string::reverse_iterator y_iter = y.rbegin();

    string::reverse_iterator x_end = x.rend();
    string::reverse_iterator y_end = y.rend();

    while (x_iter < x_end && y_iter < y_end) {
        ulong d1 = (ulong) ((*x_iter) - '0');
        ulong d2 = (ulong) ((*y_iter) - '0');

        long sub = d1 - d2 - memory;
        
        if (sub < 0) {
            sub += 10;
            memory = 1;
        }
        else {
            memory = 0;
        }

        res.push_back((char) (sub) + '0');

        x_iter++;
        y_iter++;
    }

    std::reverse(res.begin(), res.end());

    return res;
}

string karatsuba(string x, string y) {
    // For numbers which contains only 1 digit [1, .., 9]
    if (x.length() == 1 && y.length() == 1) {
        ulong x1 = std::stoul(x);
        ulong y1 = std::stoul(y);
        return std::to_string(x1 * y1);
    }

    // Make numbers be same and even length
    while (x.length() < y.length())
        x = '0' + x;
    
    while (y.length() < x.length())
        y = '0' + y;
    
    if (x.length() % 2 == 1) {
        x = '0' + x;
        y = '0' + y;
    }

    usize len = x.length();
    usize half_len = len / 2;

    string x_left = x.substr(0, half_len);
    string x_right = x.substr(half_len);

    string y_left = y.substr(0, half_len);
    string y_right = y.substr(half_len);

    string r1 = karatsuba(x_left, y_left);
    string r2 = karatsuba(x_right, y_right);
    string r3 = karatsuba(add(x_left, x_right), add(y_left, y_right));
    
    r3 = sub(r3, r1);
    r3 = sub(r3, r2);
    
    // Perform 'shifting'
    for (usize i = 0; i < len; i++)
        r1.push_back('0');
    
    for (usize i = 0; i < half_len; i++)
        r3.push_back('0');

    string result = add(r1, r2);
    result = add(result, r3);

    while (result[0] == ' ')
        result.erase(result.cbegin());
    
    while (result[0] == '0' && result.length() > 1)
        result.erase(result.cbegin());

    return result;
}

vector<test_unit> make_test_units() {
    vector<test_unit> units;

    std::srand(1);

    // Creating test array
    for (usize i = 0; i < 100; i++) {
        usize number1 = std::rand() % 9000 + 1000;
        usize number2 = std::rand() % 9000 + 1000;
        usize number3 = number1 * number2;

        units.push_back({
            std::to_string(number1),
            std::to_string(number2),
            std::to_string(number3),
        });
    }

    return units;
}

int main(int, char * []) {
    vector<test_unit> tests = make_test_units();

    for (const auto & [in1, in2, out] : tests) {
        string actual = karatsuba(in1, in2);
        if (actual != out) {
            std::cout << in1 << ' ' << in2 << '\n';
            std::cout << actual << ' ' << out << '\n' << '\n';
        }
    }

    return 0;
}
