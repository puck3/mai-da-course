#include <algorithm>
#include <iostream>
#include <vector>

void dfs(size_t v, const std::vector<std::vector<size_t>>& graph,
         std::vector<bool>& check_visited, std::vector<size_t>& path) {
  path.push_back(v);
  check_visited[v] = true;
  for (size_t u : graph[v]) {
    if (!check_visited[u]) {
      dfs(u, graph, check_visited, path);
    }
  }
}

std::ostream& print_path(std::vector<size_t> path, std::ostream& os) {
  std::sort(path.begin(), path.end());
  for (size_t i : path) {
    os << i + 1 << " ";
  }
  os << std::endl;
  return os;
}

int main() {
  size_t n, m;
  std::cin >> n >> m;
  std::vector<std::vector<size_t>> graph(n);

  for (size_t i = 0; i < m; ++i) {
    size_t v, u;
    std::cin >> v >> u;
    graph[v - 1].push_back(u - 1);
    graph[u - 1].push_back(v - 1);
  }

  std::vector<bool> check_visited(n, false);
  for (size_t v = 0; v < n; ++v) {
    if (check_visited[v]) {
      continue;
    }
    std::vector<size_t> path;
    dfs(v, graph, check_visited, path);
    print_path(path, std::cout);
  }
}