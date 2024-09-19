#ifndef library_binary_heap_h
#define library_binary_heap_h

#include <cstddef>
#include <list>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <utility>

// Quite old stuff

class graph {
 public:
  using vertex = int;
  using edge = std::pair<int, int>;

  graph() = delete;

  graph(std::size_t count) {
    adj_ = new std::list<edge>[count];
    vertex_count_ = count;
  }

  // TODO: implement copy/move ctors
  graph(const graph& g) = delete;
  graph(graph&& g) = delete;

  ~graph() {}

  void add_edge(vertex u, vertex v, std::size_t weight) {}

  std::pair<int, int> calc_mst_weights() {
    // TODO: run "this->prims" method to each connection component of graph
    return std::pair(0, 0);
  }

 private:
  int prims(int start_vertex) const {
    std::priority_queue<edge, std::vector<edge>, std::greater<edge>> queue;

    std::vector<int> weights(vertex_count_, INF);
    std::vector<edge> parents(vertex_count_, std::make_pair(-1, INF));
    std::vector<bool> inTree(vertex_count_, false);

    queue.push(std::make_pair(0, start_vertex));
    weights[start_vertex] = 0;

    while (!queue.empty()) {
      int v = queue.top().second;
      queue.pop();

      inTree[v] = true;

      for (const auto& i : adj_[v]) {
        int u = i.first;
        int weight = i.second;

        if (!inTree[u] && weight < weights[u]) {
          weights[u] = weight;
          queue.push(std::make_pair(weight, u));
          parents[u] = std::make_pair(v, weight);
        }
      }
    }

    int totalWeight = 0;

    for (int i = 0; i < vertex_count_; i++) {
      if (parents[i].first == -1)
        continue;

      totalWeight += parents[i].second;
    }

    return totalWeight;
  }

  std::size_t vertex_count_;
  std::list<edge>* adj_;

  static constexpr int INF = static_cast<int>((2ull << 31) - 1);
};

#endif
