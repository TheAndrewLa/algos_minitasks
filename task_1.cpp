#include <iostream>
#include <tuple>
#include <string>
#include <vector>
#include <algorithm>

using std::string;
using std::vector;
using std::pair;

using usize = std::size_t;
using ulong = unsigned long;

// First and second is for input numbers and the third one is for expecting result (divided & remainder)
using test_unit = std::tuple<string, string, string>;

bool greater_or_equal(string x, string y) {
    if (x.length() == y.length())
        return x.compare(y) >= 0;

    return x.length() >= y.length();
}

bool less_or_equal(string x, string y) {
    if (x.length() == y.length())
        return x.compare(y) <= 0;

    return x.length() <= y.length();
}

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

    while (res[0] == ' ')
        res.erase(res.cbegin());
    
    while (res[0] == '0' && res.length() > 1)
        res.erase(res.cbegin());

    return res;
}

// Subtraction needs Y to be lower than X
string sub(string x, string y) {
    string res;
    ulong memory = 0;

    if (less_or_equal(x, y))
        return "0";

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

    while (res[0] == ' ')
        res.erase(res.cbegin());
    
    while (res[0] == '0' && res.length() > 1)
        res.erase(res.cbegin());

    return res;
}

string divide(string x, string y) {
    // Task definitions
    // Length of x is N
    // Length of y is M

    string result {"0"};
    string x_current {x};

    while (greater_or_equal(x_current, y)) {
        // Each loop iteration performs one subtraction and one addition
        // Time complexity of subtraction is O(min{len(x_current), len(y)}) = O(M), because x_current is greater than y
        // Time complexity of addition is O(min{len(result), len("1")}) = O(1)
        x_current = sub(x_current, y);
        result = add(result, "1");

        // Well, time complexity of each loop iteration is O(M)
        // It's pretty obvious that we will do (N - M) iterations of loop (while we can do subtraction)
        // This equation makes sense: N - M = O(N)
        // So the complexity of all algorithm is O(M * N)
    }

    return result;
}

vector<test_unit> make_test_unit() {
    vector<test_unit> units;

    std::srand(1);

    // Creating test array
    for (usize i = 0; i < 100; i++) {
        usize number1 = std::rand() % 10000 + 10000;
        usize number2 = std::rand() % 10000;
        usize number3 = number1 / number2;

        units.push_back({
            std::to_string(number1),
            std::to_string(number2),
            std::to_string(number3),
        });
    }

    return units;
}

int main(int, char * []) {
    vector<test_unit> tests = make_test_unit();

    for (const auto & [in1, in2, out] : tests) {
        string actual = divide(in1, in2);
        if (actual != out) {
            std::cout << in1 << ' ' << in2 << '\n';
            std::cout << actual << ' ' << out << '\n' << '\n';
        }
    }

    return 0;
}
