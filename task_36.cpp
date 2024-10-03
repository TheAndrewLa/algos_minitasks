#include <cassert>
#include <cstddef>

#include <iostream>
#include <string>

#include <vector>

struct disjoint_set;
struct task;

namespace std {
string to_string(const task& task);
string to_string(const disjoint_set& set);
}  // namespace std

struct disjoint_set {
  disjoint_set() = delete;

  disjoint_set(std::size_t size) {
    assert(static_cast<std::ptrdiff_t>(size) > 0);

    size_ = size;

    tree_sizes_ = new std::size_t[size_];
    parents_ = new unsigned[size_];

    init_set();
  }

  disjoint_set(const disjoint_set& set) : size_(set.size_) {
    tree_sizes_ = new std::size_t[size_];
    parents_ = new unsigned[size_];

    for (std::size_t i = 0; i < size_; i++) {
      tree_sizes_[i] = set.tree_sizes_[i];
      parents_[i] = set.parents_[i];
    }
  }

  disjoint_set(disjoint_set&& set) : size_(set.size_) {
    tree_sizes_ = set.tree_sizes_;
    parents_ = set.parents_;

    set.tree_sizes_ = nullptr;
    set.parents_ = nullptr;
  }

  ~disjoint_set() {
    delete[] tree_sizes_;
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

    // Union-by-size optimization

    if (tree_sizes_[setX] < tree_sizes_[setY]) {
      parents_[setX] = setY;
      tree_sizes_[setX] += tree_sizes_[setY];
    } else {
      parents_[setY] = setX;
      tree_sizes_[setY] += tree_sizes_[setX];
    }
  }

 private:
  friend std::string std::to_string(const disjoint_set&);

  void init_set() {
    for (std::size_t i = 0; i < size_; ++i) {
      tree_sizes_[i] = 0;
      parents_[i] = i;
    }
  }

  unsigned* parents_;
  std::size_t* tree_sizes_;
  std::size_t size_;
};

struct task {
  task(char id, std::size_t deadline, std::size_t penalty) : identifier_(id), deadline_(deadline), penalty_(penalty) {}

 private:
  friend std::string std::to_string(const task& task);

  char identifier_;
  std::size_t deadline_;
  std::size_t penalty_;
};

std::string std::to_string(const task& task) {
  std::string str = "Task";

  str += " ";
  str += std::string(1, task.identifier_);

  str += " ";
  str += std::to_string(task.deadline_);

  str += " ";
  str += std::to_string(task.penalty_);

  return str;
}

std::string std::to_string(const disjoint_set& set) {
  string str = "Set:";

  for (size_t i = 0; i < set.size_; ++i) {
    str += " ";
    str += to_string(set.parents_[i]);
  }

  return str;
}

void greedy_planning(const std::vector<task>& tasks, std::vector<task>& result) {}

void optimal_planning(const std::vector<task>& tasks, std::vector<task>& result) {}

int main() {
  std::vector<task> tasks;
  tasks.push_back(task('A', 3, 25));
  tasks.push_back(task('B', 4, 10));
  tasks.push_back(task('C', 1, 30));
  tasks.push_back(task('D', 3, 50));
  tasks.push_back(task('E', 3, 20));

  std::vector<task> plan1, plan2;

  greedy_planning(tasks, plan1);
  optimal_planning(tasks, plan2);

  std::cout << "GREEDY PLAN:" << std::endl;

  for (const auto& i : plan1) {
    std::cout << std::to_string(i) << std::endl;
  }

  std::cout << std::endl;

  std::cout << "OPTIMAL PLAN:" << std::endl;

  for (const auto& i : plan2) {
    std::cout << std::to_string(i) << std::endl;
  }

  std::cout << std::endl;

  return 0;
}
