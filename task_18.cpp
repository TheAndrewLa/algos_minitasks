#include <vector>

using std::vector;
using coord = std::pair<int, int>;
using coord_array = std::vector<coord>;

int kth(coord_array::iterator start, coord_array::iterator end, int target) {    
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
        return kth(i, end, target);
}

int find_optimal(vector<coord>& coords) {
    std::srand(0);
    
    return kth(coords.begin(), coords.end(), coords.size() / 2 + coords.size() % 2);
}

int main(int, char**) {
}
