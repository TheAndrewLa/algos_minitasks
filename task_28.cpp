#include <iostream>
#include <string>

#include <vector>
#include <stdexcept>

#include <cmath>
#include <cassert>

using usize = std::size_t;
using isize = std::ptrdiff_t;

using i32 = int32_t;
using u32 = uint32_t;

using i8 = int8_t;
using u8 = uint8_t;

using real32 = float;

// Just for fun

union IP {
    struct {
        u8 a, b, c, d;
    };

    u32 mask;
};

consteval bool IsPrime(usize number) {
    for (usize i = 2; i * i <= number; i++) {
        if (number % i == 0)
            return false;
    }

    return true;
}

consteval usize NearestPrime(usize number) {
    usize i = number + 1;
    while (!IsPrime(i))
        i++;
    
    return i;
}

class BloomFilter {
    public:
    BloomFilter(usize size, float error) {
        if (error <= 0.0f)
            throw std::invalid_argument{"Invalid error value!"};

        usize k = -ceilf(log2f(error));
        size_ = size * ceilf(-log2f(error) / logf(2.0f));

        bitset_ = std::vector<usize>(((size_ + sizeof(int)) / sizeof(int) + 7) / 8, 0);
        hash_values_ = std::vector<std::pair<u32, u32>>(k);

        for (auto& i : hash_values_) {
            i.first = rand() % hash_base;
            i.second = rand() % hash_base;
        }
    }

    void insert(IP addr) {
        for (const auto& i : hash_values_)
            set_bit(hash(addr.mask, i.first, i.second), true);
    }

    bool lookup(IP addr) {
        for (auto& i : hash_values_) {
            if (peek_bit(hash(addr.mask, i.first, i.second)) == 0)
                return false;
        }

        return true;
    }

    private:
    usize size_;
    std::vector<usize> bitset_;
    std::vector<std::pair<u32, u32>> hash_values_;

    const usize bitset_param = sizeof(usize) * 8;
    const usize hash_base = NearestPrime(1200);

    inline usize hash(u32 x, usize a, usize b) const {
        return ((a * x + b) % hash_base) % size_;
    }

    inline void set_bit(usize index, bool bit) {
        usize global = index / bitset_param;
        usize local = index % bitset_param;

        bitset_[global] |= (static_cast<usize>(bit) << local);
    }

    inline bool peek_bit(usize index) const {
        usize global = index / bitset_param;
        usize local = index % bitset_param;

        return ((bitset_[global] >> local) & 1);
    }
};
