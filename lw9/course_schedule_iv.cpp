#include <iostream>
#include <vector>

class Solution {
 private:
  bool dfs(int a, int b, const std::vector<std::vector<int>>& graph,
           std::vector<bool>& check_visited) {
    check_visited[a] = true;
    if (a == b) {
      return true;
    }
    for (int v : graph[a]) {
      if (!check_visited[v] && dfs(v, b, graph, check_visited)) {
        return true;
      }
    }
    return false;
  }

  std::vector<std::vector<int>> get_graph(
      int numCourses, const std::vector<std::vector<int>>& prerequisites) {
    std::vector<std::vector<int>> graph(numCourses);
    for (auto prerequisite : prerequisites) {
      graph[prerequisite[0]].push_back(prerequisite[1]);
    }
    return graph;
  }

 public:
  std::vector<bool> checkIfPrerequisite(
      int numCourses, std::vector<std::vector<int>>& prerequisites,
      std::vector<std::vector<int>>& queries) {
    auto graph = get_graph(numCourses, prerequisites);
    std::vector<bool> answer;
    for (auto query : queries) {
      std::vector<bool> check_visited(numCourses, false);
      bool res = dfs(query[0], query[1], graph, check_visited);
      answer.push_back(res);
    }
    return answer;
  }
};