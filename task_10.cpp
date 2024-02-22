#include <string>
#include <vector>
#include <array>
#include <iostream>
#include <algorithm>

using std::string;
using std::vector;
using std::array;

using usize = std::size_t;
using isize = std::ptrdiff_t;

constexpr usize capacity = 128;

void lsd_radix_iteration(vector<string>& words, array<vector<string>, capacity>& array, usize index) {
    vector<string> vec;

    for (const auto& i : words)
        array[i[index]].push_back(i);

    for (const auto& i : array)
        vec.insert(vec.end(), i.cbegin(), i.cend());

    std::copy(vec.cbegin(), vec.cend(), words.begin());
}

void lsd_radix(vector<string>& words) {
    // Algorithm steps
    // 1) Find max length of a string
    // 2) Add some null-symbols '\0' to the end of each string
    // 3) Then run a loop. First index has to be (len - 1), last index has to be 0
    // 4) And at the end we're gonna get a good array

    usize max_length = words[0].size();

    std::for_each(words.begin() + 1, words.end(), [&max_length] (string i) {
        max_length = std::max(max_length, i.size());
    });

    // Add null-symbols
    std::for_each(words.begin(), words.end(), [&max_length] (string& str) {
        for (usize i = str.size(); i < max_length; i++)
            str = '\0' + str;
    });

    // Create and tmp array once
    array<vector<string>, capacity> tmp_array;

    // Running a count sort for each 'digit' in string
    for (isize i = max_length - 1; i >= 0; i--) {
        lsd_radix_iteration(words, tmp_array, i);
        tmp_array.fill({});
    }

    // Remove null-symbols
    std::for_each(words.begin(), words.end(), [] (string& str) {
        while (str.front() == '\0')
            str.erase(str.begin());
    });
}

int main() {
    vector<string> vec {"hello", "world", "i", "love", "sys_pro", "and", "hate", "engineers"};
    lsd_radix(vec);

    for (const auto& i : vec)
        std::cout << i << '\n';
}
