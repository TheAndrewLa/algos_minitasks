#include <iostream>
#include <string>

#include <vector>
#include <stdexcept>

#include <cmath>
#include <cassert>

#include <string>

using std::string;

using usize = std::size_t;
using isize = std::ptrdiff_t;

using i32 = int32_t;
using u32 = uint32_t;

using i8 = int8_t;
using u8 = uint8_t;

using real32 = float;

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

    ip_hasher(usize base) noexcept {
        rehash(base);
    }

    void rehash(usize base) noexcept {
        base_ = base;

        a1_ = 1 + (rand() % (base - 1));
        a2_ = 1 + (rand() % (base - 1));
        a3_ = 1 + (rand() % (base - 1));
        a4_ = 1 + (rand() % (base - 1));
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

        assert(k != 0);
        assert(b != 0);

        size_ = b * size;
        bitset_ = std::vector<bool>(size_, false);

        hashers_ = std::vector<ip_hasher>(k, {hash_base});
    }

    void insert(ip_address addr) {
        for (const auto& i : hashers_)
            bitset_[i.hash(addr) % size_] = true;
    }

    bool search(ip_address addr) {
        for (auto& i : hashers_) {
            if (!bitset_[i.hash(addr) % size_])
                return false;
        }

        return true;
    }

    private:
    usize size_;
    std::vector<bool> bitset_;
    std::vector<ip_hasher> hashers_;

    const usize bitset_base = sizeof(usize) * 8;
    const usize hash_base = NearestPrime(1200);
};

void calculate() {
    std::srand(time(nullptr));

    // First = Number of elements
    u32 n;

    // Second = Erorr
    float error;

    std::cin >> n >> error;
    bloom_filter filter{n, error};

    usize false_positive = 0;

    for (u32 i = 0; i < n / 2; i++) {
        ip_address addr;
        addr.mask = i + 4000;

        if (filter.search(addr))
            false_positive++;
        
        filter.insert(addr);
    }

    std::cout << false_positive << '\n';

}

int main(int argc, char** argv) {
    while (1) {
        calculate();
    }

    return 0;
}
