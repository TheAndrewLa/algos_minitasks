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

union ip_address {
    struct {
        u8 h1, h2, h3, h4;
    };

    u32 mask;
};

constexpr bool IsPrime(usize number) {
    for (usize i = 2; i * i <= number; i++) {
        if (number % i == 0)
            return false;
    }

    return true;
}

constexpr usize NearestPrime(usize number) {
    usize i = number + 1;
    while (!IsPrime(i))
        i++;
    
    return i;
}

class ip_hasher {
    public:
    ip_hasher() = default;

    ip_hasher(usize base) {
        rehash(base);
    }
    
    void rehash(usize base) {
        base_ = base;

        a1_ = rand() % base;
        a2_ = rand() % base;
        a3_ = rand() % base;
        a4_ = rand() % base;
    }

    inline usize hash(ip_address addr) const {
        usize b1 = addr.h1 * a1_;
        usize b2 = addr.h2 * a2_;
        usize b3 = addr.h3 * a3_;
        usize b4 = addr.h4 * a4_;
        return (b1 + b2 + b3 + b4) % base_;
    }

    private:
    usize base_;

    usize a1_;
    usize a2_;
    usize a3_;
    usize a4_;
};

class bloom_filter {
    public:
    bloom_filter(usize size, float error) {
        // 0.1% is min possible error
        // 100% is max possible error (wut the fuck is wrong with you)

        if (error <= 0.001f || error >= 1.0f)
            throw std::invalid_argument{"Invalid error value!"};

        usize k = -ceilf(log2f(error)); // number of hash functions
        usize b = -ceilf(log2f(error) / logf(2.0f)); // number of bits to one object

        bitset_ = std::vector<usize>((b * size) / sizeof(usize), 0);

        std::srand(std::time(nullptr));
        hashers_ = std::vector<ip_hasher>(k, {hash_base});
    }

    void insert(ip_address addr) {
        for (const auto& i : hashers_)
            set_bit(i.hash(addr), true);
    }

    bool search(ip_address addr) {
        for (auto& i : hashers_) {
            if (!peek_bit(i.hash(addr)))
                return false;
        }

        return true;
    }

    private:
    std::vector<usize> bitset_;
    std::vector<ip_hasher> hashers_;

    const usize bitset_base = sizeof(usize) * 8;
    const usize hash_base = NearestPrime(1200);

    inline void set_bit(usize index, bool bit) {
        usize global = index / bitset_base;
        usize local = index % bitset_base;

        bitset_[global] |= (static_cast<usize>(bit) << local);
    }

    inline bool peek_bit(usize index) const {
        usize global = index / bitset_base;
        usize local = index % bitset_base;

        return ((bitset_[global] >> local) & 0x1);
    }
};
