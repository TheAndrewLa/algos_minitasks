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

     /// @todo Use smart pointers here
    struct Connections {
        BinomialTreeNode* parent;
        BinomialTreeNode* child;
        BinomialTreeNode* sibling;
    } connections;

    /// @todo Think about rule of 3/5 here

    BinomialTreeNode() = delete;
    BinomialTreeNode(const BinomialTreeNode& heap) = default;
    BinomialTreeNode(BinomialTreeNode&& heap) = default;

    ~BinomialTreeNode() = default;

    BinomialTreeNode(k key, v value) : key(key), value(value), degree(0) {}
};

template <typename V, typename K = isize, class compare_class = std::greater<K>>
class BinomialHeap {
    public:
    using Node = BinomialTreeNode<K, V>;
    using NodeData = std::pair<K, V>;

    using Key = K;
    using Value = V;

    // Following rule of 5 ('cause im c++ guy)

    BinomialHeap() noexcept {
    }

    explicit BinomialHeap(const NodeData& node) noexcept {
    }

    BinomialHeap(const BinomialHeap& heap) {
    }

    BinomialHeap(BinomialHeap&& heap) {
    }

    ~BinomialHeap() {
        this->clear();
    }

    BinomialHeap& operator=(const BinomialHeap& heap);
    BinomialHeap& operator=(BinomialHeap&& heap);

    // Merging

    void merge(const BinomialHeap& heap) {
        if (!heap.validate())
            throw std::invalid_argument("Another heap is broken, merge can't be done!");
    }

    static BinomialHeap merge(const BinomialHeap& h1, const BinomialHeap& h2) {
        if (!(h1.validate() && h2.validate))
            throw std::invalid_argument("Heaps are broken, merge can't be done");
    }

    // Inserting node by key & value

    void insert_node(const NodeData& node) {
        this->merge(BinomialHeap {node});
    }

    // Peeking and popping min element

    NodeData pop_top() {
        if (head_ == nullptr)
            throw std::logic_error("You can't peek element from empty heap!");
    }

    NodeData peek_top() const {
        if (head_ == nullptr)
            throw std::logic_error("You can't peek element from empty heap!");

        Node* min {head_};
        Node* current {head_->connections.sibling};

        while (current != nullptr) {
            if (compare(min->key, current->key))
                min = current;

            current = current->connections.sibling;
        }

        return NodeData {min->key, min->value};
    }

    /// @todo Implement method of converting heap to string
    std::string to_string() const {
    }

    /// @todo Implement validating method
    bool validate() const {
    }

    /// @todo Implement method of total releasing of heap
    void clear() {
    }

    private:
    const compare_class compare;

    usize get_siblings_count(Node* node) const {
        usize total {0};
        Node* iter {head_};

        while (iter != nullptr) {
            total++;
            iter = iter->connections.sibling;
        }

        return total;
    }

    void release_tree(Node* tree_head) {}

    // Bitmask of heights of our binomial trees
    std::bitset<64> trees_;
    Node* head_;
};
