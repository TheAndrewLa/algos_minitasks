#include <iostream>
#include "library/graph.hpp"

int main(int, char**) {
  graph g(5);
  g.add_edge(0, 1, 3);
  g.add_edge(0, 4, 1);
  g.add_edge(1, 4, 4);
  g.add_edge(1, 2, 5);
  g.add_edge(2, 3, 2);
  g.add_edge(3, 4, 7);
  g.add_edge(2, 4, 6);

  std::cout << g.run_prims(0) << std::endl;
  return 0;
}
