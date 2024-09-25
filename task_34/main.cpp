#include <iostream>
#include "library/graph.hpp"

#ifdef TEST

int main(void) {
  int vertex_count, edge_count;
  std::cin >> vertex_count >> edge_count;

  graph g(vertex_count);

  for (int i = 0; i < edge_count; ++i) {
    int u, v, weight;
    std::cin >> u >> v >> weight;

    g.add_edge(u, v, weight);
  }

  std::pair<int, int> result = g.calc_mst_weights();
  std::cout << result.first << ' ' << result.second << std::endl;

  return 0;
}

#else

int main(void) {
  return 0;
}

#endif
