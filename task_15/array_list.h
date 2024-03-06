#pragma once
#include <stdexcept>
#include <type_traits>
#include <new>
#include <cstdlib>
#include <cassert>
#include <iostream>

template <typename t>
class ArrayList {
    public:
    using size_type = std::size_t;
    
    private:
    size_type size_;
    size_type capacity_;
    t* elements_;

    static inline void npe_(void* ptr) {
        if (ptr == nullptr)
            throw std::bad_alloc();
    }

    public:
    ArrayList() : ArrayList(16) {}

    explicit ArrayList(size_type cap) : size_(0) {
        // Some type checks
        static_assert(std::is_copy_assignable_v<t>, "Array list have to copy elements");
        static_assert(std::is_destructible_v<t>, "Array list have to destruct elements");

        if (cap == 0)
            throw std::invalid_argument("Capacity should be positive");
        
        capacity_ = cap;
        elements_ = reinterpret_cast<t*>(std::malloc(sizeof(t) * cap));
        npe_(elements_);
    }

    ~ArrayList() {
        for (size_type i = 0; i < size_; i++)
            elements_[i].~t();

        std::free(elements_);
    }

    t& get(size_type index) const {
        assert(size_ <= capacity_ && capacity_ > 0 && elements_ != nullptr);

        if (index >= size_)
            throw std::out_of_range("Array index is out of range");

        return elements_[index];
    }

    t& front() const {
        assert(size_ <= capacity_ && capacity_ > 0 && elements_ != nullptr);

        if (size_ == 0)
            throw std::length_error("Array is empty");

        return elements_[0]; 
    }

    t& back() const {
        assert(size_ <= capacity_ && capacity_ > 0 && elements_ != nullptr);

        if (size_ == 0)
            throw std::length_error("Array is empty");

        return elements_[size_ - 1]; 
    }

    void push_back(const t& value) {
        assert(size_ <= capacity_ && capacity_ > 0 && elements_ != nullptr);

        if (size_ == capacity_) {
            capacity_ *= 2;

            elements_ = reinterpret_cast<t*>(std::realloc(elements_, sizeof(t) * capacity_));
            npe_(elements_);
        }

        elements_[size_++] = value;
    }

    void pop_back() {
        assert(size_ <= capacity_ && capacity_ > 0 && elements_ != nullptr);

        if (4 * size_ == capacity_) {
            capacity_ /= 2;

            elements_ = reinterpret_cast<t*>(std::realloc(elements_, sizeof(t) * capacity_));
            npe_(elements_);
        }

        if (size_ == 0)
            throw std::length_error("Array is empty");

        elements_[size_ - 1].~t();
        size_--;
    }

    inline size_type length() const noexcept {
        return size_;
    }

    inline size_type capacity() const noexcept {
        return capacity_;
    }
};
