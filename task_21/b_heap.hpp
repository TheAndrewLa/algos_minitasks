#include <cstdint>

#include <functional>

#include <stdexcept>

#include <forward_list>
#include <list>
#include <bitset>

#include <type_traits>

using usize = std::size_t;
using isize = std::ptrdiff_t;

using u32 = std::uint32_t;
using i32 = std::int32_t;

template <typename k, typename v>
struct BinomialTreeNode {
    k key;
    v value;

    usize degree;

    struct Connections {
        BinomialTreeNode* parent;
        BinomialTreeNode* child;
        BinomialTreeNode* sibling;
    } connections;
};

template <typename value, typename key = isize, class compare_class = std::greater<key>>
class BinomialHeap {
    public:
    using Node = BinomialTreeNode<key, value>;
    using NodeFields = std::pair<key, value>;

    using Key = key;
    using Value = value;

    // Following rule of 5 ('cause im c++ guy)

    BinomialHeap() noexcept {}
    BinomialHeap(const BinomialHeap& heap) {}
    BinomialHeap(BinomialHeap&& heap) {}

    ~BinomialHeap();

    BinomialHeap& operator=(const BinomialHeap& heap);
    BinomialHeap& operator=(BinomialHeap&& heap);

    // Merging (by operator or by method)

    BinomialHeap& operator+=(const BinomialHeap& heap) {
        this->merge(heap);
        return *this;
    }

    void merge(const BinomialHeap& heap) {
        if (!heap.validate())
            throw std::logic_error("Another heap is broken, merge can't be done!");
    }

    // Inserting node by key & value

    void insert_node(const NodeFields& node) {}

    // Peeking and popping min element

    NodeFields pop_top() {
        if (head_ == nullptr)
            throw std::logic_error("You can't peek element from empty heap!");
    }

    NodeFields peek_top() const {
        if (head_ == nullptr)
            throw std::logic_error("You can't peek element from empty heap!");
    }

    // Convert heap to string

    std::string to_string() const;

    // Method to check if our binomial heap is correct
    bool validate() const;

    private:
    const compare_class compare {};

    std::bitset<64> trees_ {0};
    Node* head_ {nullptr};
};
