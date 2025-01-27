#include <iostream>
#include <vector>

class Knapsack {
 private:
  u_int64_t cost;
  std::vector<u_int64_t> items;

 public:
  Knapsack() : cost(0), items() {}

  Knapsack(u_int64_t cost, u_int64_t item) : cost(cost), items() {
    items.push_back(item);
  }

  Knapsack(u_int64_t cost, const std::vector<u_int64_t>& items)
      : cost(cost), items(items) {}

  u_int64_t getValue() const noexcept { return cost * items.size(); }

  const std::vector<u_int64_t>& getItems() const noexcept { return items; }

  Knapsack operator+(const Knapsack& other) const {
    u_int64_t cost = this->cost + other.cost;
    std::vector<u_int64_t> items{this->items};
    for (auto item : other.items) {
      items.push_back(item);
    }
    return Knapsack(cost, items);
  }

  bool operator>(const Knapsack& other) const noexcept {
    return this->getValue() > other.getValue();
  }
};

std::vector<std::vector<Knapsack>> calculateItem(
    u_int64_t n, u_int64_t m, u_int64_t item, u_int64_t weight, u_int64_t cost,
    const std::vector<std::vector<Knapsack>>& previousRow) {
  std::vector<std::vector<Knapsack>> row(
      m + 1, std::vector<Knapsack>(n + 1, Knapsack()));
  for (u_int64_t w = 1; w <= m; ++w) {
    for (u_int64_t k = 1; k <= n; ++k) {
      if (weight <= w) {
        Knapsack withoutCurrentItem = previousRow[w][k];
        Knapsack withCurrentItem =
            previousRow[w - weight][k - 1] + Knapsack(cost, item);
        if (withCurrentItem.getItems().size() == k &&
            withCurrentItem > withoutCurrentItem) {
          row[w][k] = withCurrentItem;
        } else {
          row[w][k] = withoutCurrentItem;
        }
      } else {
        row[w][k] = previousRow[w][k];
      }
    }
  }
  return row;
}

Knapsack calculateKnapsack(u_int64_t n, u_int64_t m) {
  std::vector<std::vector<Knapsack>> dp_i(
      m + 1, std::vector<Knapsack>(n + 1, Knapsack()));

  for (u_int64_t i = 1; i <= n; ++i) {
    u_int64_t weight, cost;
    std::cin >> weight >> cost;
    dp_i = calculateItem(n, m, i, weight, cost, dp_i);
  }
  Knapsack bestKnapsack = dp_i[m][0];
  for (Knapsack knapsack : dp_i[m]) {
    if (knapsack > bestKnapsack) {
      bestKnapsack = knapsack;
    }
  }
  return bestKnapsack;
}

int main() {
  u_int64_t n, m;
  std::cin >> n >> m;
  Knapsack solution = calculateKnapsack(n, m);
  std::cout << solution.getValue() << "\n";
  for (u_int64_t item : solution.getItems()) {
    std::cout << item << " ";
  }
  std::cout << "\n";
}