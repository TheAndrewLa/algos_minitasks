#include <cstdint>
#include <type_traits>
#include <functional>

using usize = std::size_t;
using isize = std::ptrdiff_t;

using u32 = std::uint32_t;
using i32 = std::int32_t;

template <typename K, typename V>
struct BinomialTreeNode {
    K key;
    V value;

    usize degree;

    struct connections {
        BinomialTreeNode* parent;
        BinomialTreeNode* child;
        BinomialTreeNode* sibling;
    };
};

template <typename V, typename K = isize, class Comparator = std::greater<K>>
class BinomialHeap {
    public:
    using NodeType = BinomialTreeNode<K, V>;
    using NodeData = std::pair<K, V>;

    using KeyType = K;
    using ValueType = V;
    using ComparatorType = decltype(Comparator);

    // Following rule of 5 ('cause im c++ guy)

    BinomialHeap() : head_(nullptr) {}
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

    void merge(const BinomialHeap& heap);

    // Inserting node by key & value

    void insert_node(const NodeData& node);

    // Peeking and popping min element

    NodeData pop_top();

    NodeData peek_top() const;

    // Convert heap to string

    std::string to_string() const;

    private:
    struct TreeList {
        NodeType* tree;
        TreeList* next;
    };

    TreeList* head_;
};
