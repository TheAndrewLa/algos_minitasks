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

    BinomialHeap() = delete;
    BinomialHeap(const BinomialHeap& heap) = default;
    BinomialHeap(BinomialHeap&& heap) = default;

    ~BinomialHeap() = default;

    BinomialHeap(k key, v value) : key(key), value(value), degree(0) {}
};

template <typename value, typename key = isize, class compare_class = std::greater<key>>
class BinomialHeap {
    public:
    using Node = BinomialTreeNode<key, value>;
    using NodeData = std::pair<key, value>;

    using Key = key;
    using Value = value;

    // Following rule of 5 ('cause im c++ guy)

    BinomialHeap() noexcept {
    } // TODO: static asserts here (check key & value and comparator)

    BinomialHeap(const NodeData& node) noexcept {
    } // TODO: static asserts here (check key & value and comparator)

    BinomialHeap(const BinomialHeap& heap) {}
    BinomialHeap(BinomialHeap&& heap) {}

    ~BinomialHeap();

    BinomialHeap& operator=(const BinomialHeap& heap);
    BinomialHeap& operator=(BinomialHeap&& heap);

    // Merging

    void merge(const BinomialHeap& heap) {
        if (!heap.validate())
            throw std::logic_error("Another heap is broken, merge can't be done!");
    }

    static BinomialHeap merge(const BinomialHeap& h1, const BinomialHeap& h2) {
        
    }

    // Inserting node by key & value

    void insert_node(const NodeData& node) {}

    // Peeking and popping min element

    NodeData pop_top() {
        if (head_ == nullptr)
            throw std::logic_error("You can't peek element from empty heap!");
    }

    NodeData peek_top() const {
        if (head_ == nullptr)
            throw std::logic_error("You can't peek element from empty heap!");

        Node* min = head_;
        Node* current = head_->connections.sibling;

        while (current != nullptr) {
            if (compare(min->key, current->key))
                min = current;

            current = current->connections.sibling;
        }

        return NodeData {min->key, min->value};
    }

    std::string to_string() const {
    } // @todo Implement method of converting heap to string

    bool validate() const {
    } /// @todo Implement validating method

    void clear() {
    } /// @todo Implement method of total releasing of heap

    private:
    const compare_class compare {};

    usize get_siblings_count(Node* node) const {
        usize total {0};
        Node* iter = head_;

        while (iter != nullptr) {
            total++;
            iter = iter->connections.sibling;
        }

        return total;
    }

    // Bitmask of heights of our binomial trees
    std::bitset<64> trees_ {0};
    Node* head_ {nullptr};
};
