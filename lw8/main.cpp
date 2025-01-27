#include <exception>
#include <iostream>
#include <vector>

std::vector<std::vector<double>> inputAdditivesMatrix(size_t n, size_t m) {
  std::vector<std::vector<double>> additives(n, std::vector<double>(m + 2, 0));
  for (size_t i = 0; i < n; ++i) {
    for (size_t j = 0; j < m + 1; ++j) {
      std::cin >> additives[i][j];
    }
    additives[i][m + 1] = i + 1;
  }
  return additives;
}

int64_t findComponentMinPrice(size_t startRow, size_t component,
                              const std::vector<std::vector<double>>& additives,
                              size_t priceColumn) {
  double minPrice = 51;
  int minPriceIndex = -1;
  for (size_t i = startRow; i < additives.size(); ++i) {
    if (additives[i][component] != 0 && additives[i][priceColumn] < minPrice) {
      minPrice = additives[i][priceColumn];
      minPriceIndex = i;
    }
  }
  return minPriceIndex;
}

void substractRow(const std::vector<double>& row,
                  std::vector<std::vector<double>>& additives, size_t startRow,
                  size_t startColumn, size_t componentsCount) {
  for (size_t i = startRow; i < additives.size(); ++i) {
    double coef = additives[i][startColumn] / row[startColumn];
    for (size_t j = startColumn; j < componentsCount; ++j) {
      additives[i][j] -= row[j] * coef;
    }
  }
}

bool gaussianElimination(std::vector<std::vector<double>>& additives, size_t n,
                         size_t m) {
  size_t j = 0;
  for (size_t i = 0; i < n; ++i) {
    int64_t rowIndex = findComponentMinPrice(i, j, additives, m);
    if (rowIndex < 0) {
      break;
    }
    std::swap(additives[i], additives[rowIndex]);
    substractRow(additives[i], additives, i + 1, j, m);
    if (++j >= m) {
      return true;
    }
  }
  return false;
}

std::vector<size_t> countingSort(const std::vector<size_t>& numbers,
                                 size_t maxKey) {
  std::vector<size_t> sortedNumbers(numbers.size(), 0);
  std::vector<size_t> countingArray(maxKey + 1, 0);
  for (size_t i = 0; i < numbers.size(); ++i) {
    ++countingArray[numbers[i]];
  }
  for (size_t i = 1; i <= maxKey; ++i) {
    countingArray[i] += countingArray[i - 1];
  }
  for (int i = numbers.size() - 1; i >= 0; --i) {
    sortedNumbers[countingArray[numbers[i]] - 1] = numbers[i];
    --countingArray[numbers[i]];
  }
  return sortedNumbers;
}

int main() {
  size_t N, M;
  std::cin >> M >> N;
  std::vector<std::vector<double>> additives = inputAdditivesMatrix(M, N);
  if (!gaussianElimination(additives, M, N)) {
    std::cout << -1 << std::endl;
  } else {
    std::vector<size_t> additivesNumbers;
    for (size_t i = 0; i < N; ++i) {
      additivesNumbers.push_back(static_cast<size_t>(additives[i][N + 1]));
    }
    for (size_t number : countingSort(additivesNumbers, M)) {
      std::cout << number << " ";
    }
    std::cout << std::endl;
  }
}
