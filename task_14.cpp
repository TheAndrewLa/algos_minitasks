#include <vector>
#include <iostream>

using isize = std::ptrdiff_t;

using std::vector;
using std::pair;
using coord = pair<int, int>;
using coord_array = std::vector<coord>;

int kth(coord_array::iterator start, coord_array::iterator end, int target) {
    isize dist = std::distance(start, end);

    if (dist <= 1) {
        return start->second;
    }

    auto pivot = start + (std::rand() % dist);
    int pivot_value = pivot->second;

    auto i = start;
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
        return kth(i + 1, end, target - i->second);
}

int find_optimal(vector<coord>& coords) {
    std::srand(100);
    return kth(coords.begin(), coords.end(), (coords.size() / 2) + (coords.size() % 2));
}

int main() {
    coord_array coords{
        {7, 2},
        {8, 7},
        {4, 3},
        {6, 5},
        {7, 1},
        {8, 8},
    };

    int a = find_optimal(coords);

    std::cout << a << std::endl;
}
