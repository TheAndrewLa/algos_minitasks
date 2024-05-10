#pragma once

#include <cstdint>
#include <cassert>

#include <stdexcept>

#include <functional>
#include <type_traits>

using usize = std::size_t;
using isize = std::ptrdiff_t;

using u32 = std::uint32_t;
using i32 = std::int32_t;

template <typename V, typename P = isize, class CMP = std::greater<P>>
class BinomialHeap;

template <typename V, typename P = isize, class CMP = std::greater<P>>
struct BinomialTree;

template <typename V, typename P, class CMP>
struct BinomialTree {
    static_assert(std::is_base_of_v<std::binary_function<P, P, bool>, CMP>);

    static_assert(std::is_copy_constructible_v<P>);
    static_assert(std::is_copy_assignable_v<P>);
    static_assert(std::is_move_constructible_v<P>);
    static_assert(std::is_move_assignable_v<P>);
    static_assert(std::is_destructible_v<P>);

    static_assert(std::is_copy_constructible_v<V>);
    static_assert(std::is_copy_assignable_v<V>);
    static_assert(std::is_move_constructible_v<V>);
    static_assert(std::is_move_assignable_v<V>);
    static_assert(std::is_destructible_v<V>);

    BinomialTree() = delete;

    BinomialTree(const BinomialTree& heap) {
        assert(heap.rels_.parent == nullptr);

        priority_ = heap.priority_;
        value_ = heap.value_;

        degree_ = heap.degree_;

        rels_.child = (heap.rels_.child == nullptr)? nullptr : heap.rels_.child->copy();
        rels_.sibling = (heap.rels_.sibling == nullptr)? nullptr : heap.rels_.sibling->copy();
    }

    BinomialTree(BinomialTree&& heap) {
        assert(heap.rels_.parent == nullptr);

        priority_ = std::move(heap.priority_);
        value_ = std::move(heap.value_);

        degree_ = heap.degree_;

        rels_.parent = heap.rels_.parent;
        rels_.child = heap.rels_.child;
        rels_.sibling = heap.rels_.sibling;

        heap.rels_.parent = nullptr;
        heap.rels_.child = nullptr;
        heap.rels_.sibling = nullptr;
    }

    ~BinomialTree() noexcept {
        delete rels_.child;
        delete rels_.sibling;
    }

    BinomialTree(const V& value, const P& priority) : value_(value), priority_(priority) {}

    BinomialTree& operator=(const BinomialTree<V, P, CMP>& tree) = delete;
    BinomialTree& operator=(BinomialTree<V, P, CMP>&& tree) = delete;

    void decrease_priority(const P& new_priority) {
        const CMP compare;

        if (compare(priority_, new_priority) == false)
            throw std::invalid_argument("Priority is not actually decreased");

        while (rels_.parent != nullptr && compare(rels_.parent->priority, new_priority)) {
            BinomialTree<V, P, CMP>* child_prev = rels_.child;
            BinomialTree<V, P, CMP>* sibling_prev = rels_.sibling;

            rels_.parent = rels_.parent->rels_.parens;
            rels_.sibling = rels_.parent->rels_.sibling;
            rels_.child = rels_.parent;

            rels_.parent->rels_.parent = this;
            rels_.parent->rels_.sibling = sibling_prev;
            rels_.parent->rels_.child = child_prev;
        }

        priority_ = new_priority;
    }

    friend class BinomialHeap<V, P, CMP>;

    private:
    P priority_;
    V value_;

    usize degree_ = 0;

    struct {
        BinomialTree<V, P, CMP>* parent{nullptr};
        BinomialTree<V, P, CMP>* child{nullptr};
        BinomialTree<V, P, CMP>* sibling{nullptr};
    } rels_;

    void merge(const BinomialTree<V, P, CMP>* tree) {

        // Make sure that we're working with trees, not subtrees
        assert(rels_.parent == nullptr);
        assert(tree->rels_.parent == nullptr);

        if (tree->degree_ != degree_)
            throw std::invalid_argument("Trees don't have same degree!");

        const CMP compare;

        BinomialTree<V, P, CMP>* sibling = rels_.sibling;

        if (compare(tree->priority_, priority_)) {

        }
        else {
        }

        degree_++;
    }

    BinomialTree<V, P, CMP>* copy() const {
        auto new_tree = new BinomialTree<V, P, CMP>{priority_, value_};

        new_tree->rels_.child = (rels_.child == nullptr)? nullptr : rels_.child->copy();
        new_tree->rels_.sibling = (rels_.sibling == nullptr)? nullptr : rels_.sibling->copy();

        return new_tree;
    }
};

template <typename V, typename P, class CMP>
class BinomialHeap {
    public:
    static_assert(std::is_base_of_v<std::binary_function<P, P, bool>, CMP>);

    static_assert(std::is_copy_constructible_v<P>);
    static_assert(std::is_copy_assignable_v<P>);
    static_assert(std::is_move_constructible_v<P>);
    static_assert(std::is_move_assignable_v<P>);
    static_assert(std::is_destructible_v<P>);

    static_assert(std::is_copy_constructible_v<V>);
    static_assert(std::is_copy_assignable_v<V>);
    static_assert(std::is_move_constructible_v<V>);
    static_assert(std::is_move_assignable_v<V>);
    static_assert(std::is_destructible_v<V>);

    using Tree = BinomialTree<V, P, CMP>;

    BinomialHeap() = delete;

    explicit BinomialHeap(const V& value, const P& priority) : head_(new Tree{value, priority}) {}

    BinomialHeap(const BinomialHeap& heap) {
        head_ = heap.head_->copy();
    }

    BinomialHeap(BinomialHeap&& heap) {
        head_ = heap.head_;
        heap.head_ = nullptr;
    }

    ~BinomialHeap() noexcept {
        delete head_;
    }

    BinomialHeap& operator=(const BinomialHeap& heap) = delete;
    BinomialHeap& operator=(BinomialHeap&& heap) = delete;

    void merge(const BinomialHeap<V, P, CMP>& heap) {
        assert(head_ != nullptr);
    }

    void insert(const P& priority, const V& value) {
        assert(head_ != nullptr);

        merge({priority, value});
    }

    V extract_top() {
        assert(head_ != nullptr);

        Tree* top = head_;
        Tree* top_prev = nullptr;

        Tree* cur = head_->rels_.sibling;
        Tree* prev = head_;

        while (cur != nullptr) {
            if (compare(top->priority_, cur->priority_)){
                top = cur;
                top_prev = prev;
            }

            prev = cur;
            cur = cur->rels_.sibling;
        }

        if (top_prev == nullptr) {
            assert(top == head_);
            head_ = top->rels_.sibling;
        }
        else {
            assert(top_prev->rels_.sibling == top);
            top_prev->rels_.sibling = top->rels_.sibling;
        }

        // top = tree to be eliminated
    }

    V peek_top() const {
        assert(head_ != nullptr);

        Tree* top = head_;
        Tree* cur = head_->rels_.sibling;

        while (cur != nullptr) {
            if (compare(top->priority_, cur->priority_))
                top = cur;
            
            cur = cur->rels_.sibling;
        }

        return top->value_;
    }

    private:
    void validate() const {
        assert(head_ != nullptr);

        Tree* cur = head_;
        Tree* next = head_->rels_.sibling;

        while (next != nullptr) {
            assert(cur->degree_ < next->degree_);

            cur = next;
            next = next->rels_.sibling;
        }
    }

    const CMP compare;
    Tree* head_;
};
