#pragma once

#include <cstdint>
#include <cassert>

#include <stdexcept>

#include <bitset>
#include <vector>

#include <functional>
#include <type_traits>

using usize = std::size_t;
using isize = std::ptrdiff_t;

using u32 = std::uint32_t;
using i32 = std::int32_t;

template <std::destructible, std::destructible, typename>
class BinomialHeap;

template <std::destructible, std::destructible, typename>
class BinomialTree;

template <std::destructible V, std::destructible P = isize, typename CMP = std::greater<P>>
class BinomialTree {
    public:
    BinomialTree() = delete;

    BinomialTree(const BinomialTree& heap) requires(std::copyable<P> && std::copyable<V>) {
        assert(heap.rels_.parent == nullptr);

        priority_ = heap.priority_;
        value_ = heap.value_;

        degree_ = heap.degree_;

        rels_.child = (heap.rels_.child == nullptr)? nullptr : heap.rels_.child->copy();
        rels_.sibling = (heap.rels_.sibling == nullptr)? nullptr : heap.rels_.sibling->copy();
    }

    BinomialTree(BinomialTree&& heap) requires(std::movable<P> && std::movable<V>) {
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
        delete rels_.sibling;
        delete rels_.child;
    }

    BinomialTree(const V& value, const P& priority) requires(std::copyable<V> && std::copyable<P>)
    : value_(value), priority_(priority) {}

    BinomialTree(V&& value, P&& priority) requires(std::movable<V> && std::movable<P>)
    : value_(value), priority_(priority) {}

    template <typename... Args>
    BinomialTree(Args&&... args) {
        auto [value_, priority_] = std::make_pair<V, P>(std::forward<Args>(args)...);
    }

    BinomialTree& operator=(const BinomialTree& tree) = delete;
    BinomialTree& operator=(BinomialTree&& tree) = delete;

    /* Decrease priority method is implemented, but i dont wanna it to be here
    void decrease_priority(const P& new_priority) {
        const CMP compare;

        if (compare(priority_, new_priority) == false)
            throw std::invalid_argument("Priority is not actually decreased");

        while (rels_.parent != nullptr && compare(rels_.parent->priority, new_priority)) {
            BinomialTree* child_prev = rels_.child;
            BinomialTree* sibling_prev = rels_.sibling;

            rels_.parent = rels_.parent->rels_.parens;
            rels_.sibling = rels_.parent->rels_.sibling;
            rels_.child = rels_.parent;

            rels_.parent->rels_.parent = this;
            rels_.parent->rels_.sibling = sibling_prev;
            rels_.parent->rels_.child = child_prev;
        }

        priority_ = new_priority;
    }
    */

    friend class BinomialHeap<V, P, CMP>;

    private:
    P priority_;
    V value_;

    usize degree_ = 0;

    struct {
        BinomialTree<V, P, CMP>* parent = nullptr;
        BinomialTree<V, P, CMP>* child = nullptr;
        BinomialTree<V, P, CMP>* sibling = nullptr;
    } rels_;

    void merge(BinomialTree* another, const CMP& compare) {
        // Make sure that we're working with trees, not subtrees
        assert(rels_.parent == nullptr);
        assert(another->rels_.parent == nullptr);

        if (another->degree_ != degree_)
            throw std::invalid_argument("Trees don't have same degree!");

        BinomialTree* tree = another->copy();

        if (compare(tree->priority_, priority_)) {
            rels_.sibling = tree->rels_.child;
            tree->rels_.child = this;
            this = tree;
        }
        else {
            tree->rels_.sibling = rels_.child;
            rels_.child = tree;
        }

        degree_++;
    }

    BinomialTree* copy() const {
        // Make sure that we're working with trees, not subtrees
        assert(rels_.parent == nullptr);

        auto new_tree = new BinomialTree{priority_, value_};

        new_tree->rels_.child = (rels_.child == nullptr)? nullptr : rels_.child->copy();
        new_tree->rels_.sibling = (rels_.sibling == nullptr)? nullptr : rels_.sibling->copy();

        return new_tree;
    }
};

template <std::destructible V, std::destructible P = isize, typename CMP = std::greater<P>>
class BinomialHeap {
    public:
    using Tree = BinomialTree<V, P, CMP>;
    using Node = BinomialTree<V, P, CMP>;

    BinomialHeap() = default;

    BinomialHeap(const V& value, const P& priority) requires(std::copyable<V> && std::copyable<P>) {
        trees_.push_back(new Tree{value, priority});
    }

    BinomialHeap(const std::pair<V, P>& vp) {}

    BinomialHeap(const BinomialHeap& heap) requires(std::copyable<V> && std::copyable<P>) {
        for (const auto& i : heap.trees_)
            trees_.push_back(i->copy());
    }

    BinomialHeap(BinomialHeap&& heap) {
        for (const auto& i : heap.trees_)
            trees_.push_back(i);

        for (auto& i : heap.trees_)
            i = nullptr;
    }

    ~BinomialHeap() noexcept {
        for (auto& i : trees_)
            delete i;
    }

    BinomialHeap& operator=(const BinomialHeap& heap) = delete;
    BinomialHeap& operator=(BinomialHeap&& heap) = delete;

    void merge(const BinomialHeap& heap) {
        BinomialTree* carry = nullptr;
        std::vector<Tree*> new_trees;
    }

    void insert(const P& priority, const V& value) {
        merge(BinomialHeap{priority, value});
    }

    V&& extract_top() {
        auto top = std::begin(trees_);
        auto end = std::end(trees_);

        for (auto i = std::next(top); i != end; ++i) {
            if (compare((*top)->priority, (*i)->priority_))
                top = i;
        }

        // top = tree node to be deleted
        // all children if top = trees to be inserted = binomial heap to be merged

        BinomialTree* top_tree = *top;
        BinomialHeap new_heap;

        usize degree = top_tree.;

        for (usize i = 0; i < degree; ++i) {
            new_heap.degrees_[i] = 1;
        }

        merge(new_heap);

        return std::move((*top)->value);
    }

    const V& peek_top() const {
        auto top = std::cbegin(trees_);
        auto end = std::cend(trees_);

        for (auto i = std::next(top); i != end; ++i) {
            if (compare((*top)->priority, (*i)->priority_))
                top = i;
        }

        return (*i)->value;
    }

    private:

    const CMP compare;
    std::vector<Tree*> trees_;
};
