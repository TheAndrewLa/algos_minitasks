#include <vector>
#include <iostream>

using std::vector;
using coord = std::pair<int, int>;
using coord_array = std::vector<coord>;

// A pretty bad variant of hoare partition.
// But all coords are unique. So, we can use this algorithm

int kth(coord_array::iterator start, coord_array::iterator end, int target) {
    if (end - start) {
        return start->second;
    }

    auto pivot = start + (std::rand() % std::distance(start, end));
    int pivot_value = pivot->second;

    auto i = start - 1;
    auto j = end;

    while (true) {
        do {
            i++;
        } while (i->second < pivot_value);

        do {
            j--;
        } while (j->second > pivot_value);

        if (i >= j)
            break;

        std::swap(*i, *j);
    }

    if (i->second == target)
        return i->second;
    else if (i->second > target)
        return kth(start, i, target);
    else
        return kth(i, end, target - i->second);
}

int find_optimal(vector<coord>& coords) {
    std::srand(100);
    return kth(coords.begin(), coords.end(), (coords.size() / 2) + (coords.size() % 2));
}

int main() {
    coord_array coords{
        {4, 4},
        {6, 6},
        {7, 2},
        {8, 7},
        {4, 3},
        {6, 5},
        {7, 1},
        {8, 8},
    };

    int a = find_optimal(coords); // Returns 4 (Correct answer)

    std::cout << a << std::endl;
}
