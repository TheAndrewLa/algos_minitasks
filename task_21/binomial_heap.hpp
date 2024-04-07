#pragma once

#include <cstdint>
#include <cassert>

#include <functional>

#include <stdexcept>

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
        BinomialTreeNode* parent {nullptr};
        BinomialTreeNode* child {nullptr};
        BinomialTreeNode* sibling {nullptr};
    } rels;

    /// @todo Think about rule of 3/5 here

    BinomialTreeNode() = delete;
    BinomialTreeNode(const BinomialTreeNode& heap) = default;
    BinomialTreeNode(BinomialTreeNode&& heap) = default;

    ~BinomialTreeNode() = default;

    BinomialTreeNode(const k& key, const v& value) : key(key), value(value), degree(0) {}

    BinomialTreeNode& operator=(const BinomialTreeNode& heap) = delete;
    BinomialTreeNode& operator=(BinomialTreeNode&& heap) = delete;
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
        // A bunch of static asserts
        static_assert(std::is_nothrow_default_constructible_v<compare_class>);
        static_assert(std::is_nothrow_destructible_v<compare_class>);

        static_assert(std::is_nothrow_destructible_v<K>);
        static_assert(std::is_nothrow_destructible_v<V>);

        static_assert(std::is_nothrow_copy_constructible_v<K>);
        static_assert(std::is_nothrow_copy_constructible_v<V>);
    }

    explicit BinomialHeap(const NodeData& node) noexcept : BinomialHeap() {

    }

    BinomialHeap(const BinomialHeap& heap) : BinomialHeap() {
        if (!heap.validate())
            throw std::invalid_argument("Copying from invalid heap can not be done!");

        head_ = copy(heap.head_);
        trees_ = heap.trees_;
    }

    BinomialHeap(BinomialHeap&& heap) : BinomialHeap() {
        if (!heap.validate())
            throw std::invalid_argument("Moving invalid heap can not be done!");

        head_ = heap.head_;
        trees_ = heap.trees_;

        heap.head_ = nullptr;
    }

    ~BinomialHeap() {
        this->clear();
    }

    BinomialHeap& operator=(const BinomialHeap& heap) {
        if (!heap.validate())
            throw std::invalid_argument("Copying from invalid heap can not be done!");

        this->clear();

        head_ = copy(heap.head_);
        trees_ = heap.trees_;
    }

    BinomialHeap& operator=(BinomialHeap&& heap) {
        if (!heap.validate())
            throw std::invalid_argument("Moving invalid heap can not be done!");

        this->clear();

        head_ = heap.head_;
        trees_ = heap.trees_;

        heap.head_ = nullptr;
    }

    // Merging

    void merge(const BinomialHeap& heap) {
        if (!heap.validate())
            throw std::invalid_argument("Another heap is invalid, merge can't be done!");
    }

    static BinomialHeap merge(const BinomialHeap& h1, const BinomialHeap& h2) {
        if (!(h1.validate() && h2.validate))
            throw std::invalid_argument("Heaps are invalid, merge can't be done");
    }

    // Inserting node by key & value

    void insert(const NodeData& node) {
        merge(BinomialHeap{node});
    }

    // Peeking and popping min element

    NodeData pop_top() {
        if (head_ == nullptr)
            throw std::logic_error("Popping element from empty heap can not be done!");

        Node* min{head_};
        Node* min_prev{nullptr};

        Node* prev{head_};
        Node* current{head_->rels.sibling};

        while (current != nullptr) {
            if (compare(min->key, current->key)) {
                min = current;
                min_prev = prev;
            }

            prev = current;
            current = current->rels.sibling;
        }

        // Min node are here

        if (min_prev == nullptr) {
            assert(min == head_);
            head_ = min->rels.sibling;
        }
        else {
            assert(min_prev->rels.sibling == min);
            min_prev->rels.sibling = min->rels.sibling;
        }

        /// @todo merge with other binomial heaps
    }

    NodeData peek_top() const {
        if (head_ == nullptr)
            throw std::logic_error("Peeking element from empty heap can not be done!");

        Node* min{head_};
        Node* current{head_->rels.sibling};

        while (current != nullptr) {
            if (compare(min->key, current->key))
                min = current;

            current = current->rels.sibling;
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
        this->release(head_);
        trees_ = std::bitset<64>{0};
    };

    private:
    const compare_class compare;

    void release(Node* node) {
        if (node == nullptr)
            return;

        release_tree(node->rels.child);
        release_tree(node->rels.sibling);

        delete node;
    }

    Node* copy(Node* node, Node* parent = nullptr) {
        if (node == nullptr)
            return nullptr;

        Node* new_node = new Node{node->key, node->value};

        new_node->rels.child = copy(node->rels.child, node);
        new_node->rels.sibling = copy(node->rels.sibling, parent);
        new_node->rels.parent = parent;

        return new_node;
    }

    // Bitmask of heights of our binomial trees
    std::bitset<64> trees_{0};
    Node* head_{nullptr};
};
