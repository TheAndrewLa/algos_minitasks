#pragma once

#include <cstdint>
#include <cassert>

#include <functional>

#include <stdexcept>

#include <bitset>
#include <forward_list>

#include <type_traits>

using usize = std::size_t;
using isize = std::ptrdiff_t;

using u32 = std::uint32_t;
using i32 = std::int32_t;

template <typename V, typename P = isize, class CMP = std::greater<P>>
class BinomialHeap;

template <typename V, typename P, class CMP>
struct BinomialTree {
    static_assert(std::is_base_of_v<std::__binary_function<P, P, bool>, CMP>);

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
        assert(heap.rels_.parent == nullptr && heap.rels_.sibling == nullptr);

        priority_ = heap.priority_;
        value_ = heap.value_;

        degree_ = heap.degree_;

        rels_.child = (heap.rels_.child == nullptr)? nullptr : heap.rels_.child->copy();
    }

    BinomialTree(BinomialTree&& heap) {
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

    ~BinomialTree() {
        if (rels_.parent != nullptr || rels_.sibling != nullptr)
            return;

        delete rels_.child;
        delete rels_.sibling;
    }

    BinomialTree(const V& value, const P& priority) : value_(value), priority_(priority) {}

    BinomialTree& operator=(const BinomialTree<V, P, CMP>& tree) = delete;
    BinomialTree& operator=(BinomialTree<V, P, CMP>&& tree) = delete;

    void decrease_priority(const P& new_priority) {
        const CMP compare;

        if (compare(priority_, new_priority) == false)
            throw std::invalid_argument("Priority is not decreased");

        while (rels_.parent != nullptr && compare(rels_.parent->priority, new_priority)) {
            // do weird swapping
        }
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
        assert(rels_.parent == nullptr && rels_.sibling == nullptr);
        assert(tree->rels_.parent == nullptr && tree->rels_.sibling == nullptr);

        if (tree->degree_ != degree_)
            throw std::invalid_argument("Trees don't have same degree!");
        
        const CMP compare;

        if (compare(tree->priority_, priority_) == true) {
            BinomialTree<V, P, CMP>* prev_tree = this->copy();
            prev_tree->rels_.parent = this;

            priority_ = tree->priority_;
            value_ = tree->value_;

            if (tree->rels_.child != nullptr)
                prev_tree->rels_.sibling = tree->rels_.child->copy();
        }
        else {
            BinomialTree<V, P, CMP>* new_tree = tree->copy();

            new_tree->rels_.sibling = rels_.child;
            rels_.child = new_tree;
        }

        degree_++;
    }

    BinomialTree<V, P, CMP>* copy() const {
        auto new_tree = new BinomialTree<V, P, CMP>{priority_, value_};

        if (rels_.child != nullptr)
            new_tree->rels_.child = rels_.child->copy();

        if (rels_.sibling != nullptr)
            new_tree->rels_.sibling = rels_.sibling->copy();

        return new_tree;
    }
};

template <typename V, typename P, class CMP>
class BinomialHeap {
    public:
    static_assert(std::is_base_of_v<std::__binary_function<P, P, bool>, CMP>);

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

    explicit BinomialHeap(const V& value, const P& priority) {
        trees_.push_back(new Tree{value, priority});
        tree_mask_ = {1};
    }

    BinomialHeap(const BinomialHeap& heap) {
        for (auto i = heap.trees_.cbegin(); i < heap.trees_.cend(); i++)
            trees_.push_back(i->copy());

        tree_mask_ = heap.tree_mask_;
    }

    BinomialHeap(BinomialHeap&& heap) {
        for (auto i = heap.trees_.begin(); i < heap.trees_.end(); i++) {
            trees_.push_back((*i));
            *i = nullptr;
        }

        tree_mask_ = heap.tree_mask_;
    }

    ~BinomialHeap() {
        for (Tree* i : trees_)
            delete i;
    }

    BinomialHeap& operator=(const BinomialHeap& heap) = delete;
    BinomialHeap& operator=(BinomialHeap&& heap) = delete;

    void merge(const BinomialHeap<V, P, CMP>& heap) {
    }

    Tree* insert(const P& priority, const V& value) {
        auto new_tree = new Tree{priority, value};

        return new_tree;
    }

    V extract_top() {
        if (trees_.empty())
            throw std::logic_error("Can't extract top element from heap!");
    }

    V peek_top() const {
        if (trees_.empty())
            throw std::logic_error("Can't get top element from heap!");

        Tree* max_tree = trees_.front();

        std::for_each(trees_.cbegin() + 1, trees_.cend(), [&max_tree, this] (auto i) {
            if (this->compare(i->priority_, max_tree->priority_)) {
                max_tree = i;
            }
        });

        return max_tree->value_;
    }

    private:
    const CMP compare;

    std::vector<Tree*> trees_;
    std::bitset<64> tree_mask_;
};
