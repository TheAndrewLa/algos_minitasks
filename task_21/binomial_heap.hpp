#pragma once

#include <cstdint>
#include <cassert>

#include <stdexcept>

#include <bitset>
#include <vector>
#include <list>

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
        // delete rels_.sibling;
        // delete rels_.child;
    }

    BinomialTree(const V& value, const P& priority) requires(std::copyable<V> && std::copyable<P>)
    : value_(value), priority_(priority) {}

    BinomialTree(V&& value, P&& priority) requires(std::movable<V> && std::movable<P>)
    : value_(value), priority_(priority) {}

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

    void merge(BinomialTree* tree, const CMP& compare) {
        assert(rels_.parent == nullptr);
        assert(tree->rels_.parent == nullptr);

        if (tree->degree_ != degree_)
            throw std::invalid_argument{"Trees should have same degree!"};

        if (compare(tree->priority_, priority_)) {
            V old_value = value_;
            P old_priority = priority_;

            value_ = tree->value_;
            priority_ = tree->priority_;

            tree->value_ = old_value;
            tree->priority_ = old_priority;
        }

        tree->rels_.sibling = rels_.child;
        rels_.child = tree;

        degree_++;
    }

    BinomialTree* copy() const requires(std::copyable<V> && std::copyable<P>) {
        assert(rels_.parent == nullptr);

        auto new_tree = new BinomialTree{value_, priority_};

        new_tree->degree_ = degree_;

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

    BinomialHeap(const V& value, const P& priority) requires(std::copyable<V> && std::copyable<P>) {
        trees_.push_front(new Tree{value, priority});
    }

    BinomialHeap(const BinomialHeap& heap) requires(std::copyable<V> && std::copyable<P>) {
        for (const auto& i : heap.trees_)
            trees_.push_back(i->copy());
    }

    BinomialHeap(BinomialHeap&& heap) {
        for (auto& i : heap.trees_) {
            trees_.push_back(i);
            i = nullptr;
        }
    }

    ~BinomialHeap() noexcept {
        for (auto& i : trees_)
            delete i;
    }

    void merge(const BinomialHeap& heap) {
        auto iter_1 = trees_.begin();
        auto end_1 = trees_.end();
        
        auto iter_2 = heap.trees_.begin();
        auto end_2 = heap.trees_.end();

        Tree* carry = nullptr;

        std::list<Tree*> new_trees;

        while (iter_1 != end_1 && iter_2 != end_2) {
            if (carry == nullptr) {
                if ((*iter_1)->degree_ > (*iter_2)->degree_) {
                    new_trees.push_back((*iter_2)->copy());
                    iter_2++;
                }

                else if ((*iter_1)->degree_ < (*iter_2)->degree_) {
                    new_trees.push_back((*iter_1)->copy());
                    iter_1++;
                }

                else {
                    carry = (*iter_1)->copy();

                    assert(carry->degree_ == (*iter_2)->degree_);
                    
                    carry->merge(*iter_2, compare);

                    iter_1++;
                    iter_2++;
                }
            }
            else {
                if ((*iter_1)->degree_ > (*iter_2)->degree_) {
                    usize degree = (*iter_2)->degree_;

                    if (degree < carry->degree_) {
                        new_trees.push_back((*iter_2)->copy());
                    }
                    else if (degree > carry->degree_) {
                        new_trees.push_back(carry->copy());
                        delete carry;

                        carry = (*iter_2)->copy();
                    }
                    else {
                        assert(carry->degree_ == (*iter_2)->degree_);
                        carry->merge((*iter_2)->copy(), compare);
                    }

                    iter_2++;
                }

                else if ((*iter_1)->degree_ < (*iter_2)->degree_) {
                    usize degree = (*iter_1)->degree_;

                    if (degree < carry->degree_) {
                        new_trees.push_back((*iter_1)->copy());
                    }
                    else if (degree > carry->degree_) {
                        new_trees.push_back(carry->copy());
                        delete carry;

                        carry = (*iter_1)->copy();
                    }
                    else {
                        assert(carry->degree_ == (*iter_1)->degree_);
                        carry->merge((*iter_1)->copy(), compare);
                    }

                    iter_1++;
                }

                else {
                    usize degree = (*iter_1)->degree_;

                    assert((*iter_1)->degree_ == (*iter_2)->degree_);

                    if (degree + 1 < carry->degree_) {
                        Tree* iters = (*iter_1)->copy();

                        assert(iters->degree_ == (*iter_1)->degree_);
                        assert(iters->degree_ == (*iter_2)->degree_);

                        iters->merge(*iter_2, compare);

                        new_trees.push_back(iters);
                    }

                    else if (degree + 1 > carry->degree_) {
                        Tree* iters = (*iter_1)->copy();

                        assert(iters->degree_ == (*iter_1)->degree_);
                        assert(iters->degree_ == (*iter_2)->degree_);

                        iters->merge(*iter_2, compare);

                        new_trees.push_back(carry->copy());
                        delete carry;

                        carry = iters;
                    }

                    else {
                        Tree* iters = (*iter_1)->copy();

                        assert((*iter_1)->degree_ == (*iter_2)->degree_);

                        iters->merge(*iter_2, compare);

                        assert(iters->degree_ == carry->degree_);

                        carry->merge(iters, compare);
                    }

                    iter_1++;
                    iter_2++;
                }
            }
        }

        while (iter_1 != end_1) {
            if (carry == nullptr) {
                new_trees.push_back((*iter_1)->copy());
            }
            else {
                usize degree = (*iter_1)->degree_;

                if (degree < carry->degree_) {
                    new_trees.push_back((*iter_1)->copy());
                }

                else if (degree > carry->degree_) {
                    new_trees.push_back(carry->copy());
                    delete carry;
                    carry = (*iter_1)->copy();
                }
                
                else {
                    assert(carry->degree_ == (*iter_1)->degree_);
                    carry->merge((*iter_1)->copy(), compare);
                }
            }

            iter_1++;
        }

        while (iter_2 != end_2) {
            if (carry == nullptr) {
                new_trees.push_back((*iter_2)->copy());
            }
            else {
                usize degree = (*iter_2)->degree_;

                if (degree < carry->degree_) {
                    new_trees.push_back((*iter_2)->copy());
                }

                else if (degree > carry->degree_) {
                    new_trees.push_back(carry->copy());
                    delete carry;
                    carry = (*iter_2)->copy();
                }

                else {
                    assert(carry->degree_ == (*iter_2)->degree_);
                    carry->merge((*iter_2)->copy(), compare);
                }
            }
            
            iter_2++;
        }

        if (carry != nullptr)
            new_trees.push_back(carry);

        for (auto& i : trees_)
            delete i;

        trees_.clear();

        for (auto& i : new_trees) {
            trees_.push_back(i);
            i = nullptr;
        }
    }

    void insert(const V& value, const P& priority) {
        merge(BinomialHeap{value, priority});
    }

    V extract_top() {
        auto top = std::begin(trees_);
        auto end = std::end(trees_);

        for (auto i = std::next(top); i != end; ++i) {
            if (compare((*top)->priority_, (*i)->priority_)) top = i;
        }

        BinomialHeap<V, P> new_one;

        Tree* top_tree = *top;
        Tree* cur = top_tree->rels_.child;

        while (cur != nullptr) {
            new_one.trees_.push_front(cur);
            cur = cur->rels_.sibling;
        }

        merge(new_one);

        top_tree->rels_.child = nullptr;
        return top_tree->value_;
    }

    const V& peek_top() const {
        auto top = std::cbegin(trees_);
        auto end = std::cend(trees_);

        for (auto i = std::next(top); i != end; ++i) {
            if (compare((*top)->priority_, (*i)->priority_))
                top = i;
        }

        return (*top)->value_;
    }

    private:
    BinomialHeap() = default;

    const CMP compare{};
    std::list<Tree*> trees_;
};
