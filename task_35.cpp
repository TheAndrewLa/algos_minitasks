#include <cassert>
#include <cstddef>

// TODO: parameterize disjoint set with template

struct disjoint_set {
  disjoint_set() = delete;

  disjoint_set(std::size_t size) {
    assert(static_cast<std::ptrdiff_t>(size) > 0);

    size_ = size;

    ranks_ = new std::size_t[size_];
    parents_ = new unsigned[size_];

    init_set();
  }

  disjoint_set(const disjoint_set& set) : size_(set.size_) {
    ranks_ = new std::size_t[size_];
    parents_ = new unsigned[size_];

    for (std::size_t i = 0; i < size_; i++) {
      ranks_[i] = set.ranks_[i];
      parents_[i] = set.parents_[i];
    }
  }

  disjoint_set(disjoint_set&& set) : size_(set.size_) {
    ranks_ = set.ranks_;
    parents_ = set.parents_;

    set.ranks_ = nullptr;
    set.parents_ = nullptr;
  }

  ~disjoint_set() {
    delete[] ranks_;
    delete[] parents_;
  }

  // TODO: implement copy-move operators

  // disjoint_set& operator=(const disjoint_set& set) { return *this; }

  // disjoint_set& operator=(disjoint_set&& set) { return *this; }

  unsigned optimizedFind(int x) {

    // Path compression optimization - parent of element X will be set 'root'
    // Don't touch ranks in this function
    // ('Cause we don't change tree structure of set, just create short path to the root)

    if (parents_[x] != x) {
      parents_[x] = optimizedFind(parents_[x]);
    }

    return parents_[x];
  }

  void optimizedUnion(int x, int y) {
    unsigned setX = optimizedFind(x);
    unsigned setY = optimizedFind(y);

    // If sets are same => union don't need to be done

    if (setX == setY)
      return;

    // Union by ranks optimization
    // If rank of x's set is lower -> 'add' x to y's set
    // If rank of y's set is lower -> 'add' y to x's set
    // Otherwise -> choose a victim (here it's x's set) & add other element to this, increase rank

    if (ranks_[setX] < ranks_[setY]) {
      parents_[setX] = setY;
    } else if (ranks_[setX] > ranks_[setY]) {
      parents_[setY] = setX;
    } else {
      parents_[setY] = setX;
      ranks_[setX]++;
    }
  }

 private:
  void init_set() {
    for (std::size_t i = 0; i < size_; ++i) {
      ranks_[i] = 0;
      parents_[i] = i;
    }
  }

  unsigned* parents_;
  std::size_t* ranks_;
  std::size_t size_;
};
