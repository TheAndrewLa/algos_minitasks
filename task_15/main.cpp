#include "array_list.h"
#include <iostream>

int main() {
    ArrayList<int> arr;

    for (int i = 0; i < 100; i++)
        arr.push_back(i);
    
    arr.front() = 42;
    arr.get(1) = 12;

    std::cout << arr.length() << ' ' << arr.capacity() << std::endl;

    for (int i = 0; i < 80; i++)
        arr.pop_back();

    std::cout << arr.length() << ' ' << arr.capacity() << std::endl;

    for (int i = 0; i < arr.length(); i++)
        std::cout << arr.get(i) << "; ";

    std::cout << std::endl;

    return 0;
}
