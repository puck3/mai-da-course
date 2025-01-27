#include <algorithm>
#include <vector>

const int MAX_I = 1e5;

std::vector<int> countingSortDesc(const std::vector<int>& array,
                                  unsigned int maxKey) {
  std::vector<int> sortedArray(array.size(), 0);

  std::vector<int> countingArray(maxKey + 1, 0);
  for (size_t i = 0; i < array.size(); ++i) {
    ++countingArray[array[i]];
  }
  for (size_t i = 1; i <= maxKey; ++i) {
    countingArray[i] += countingArray[i - 1];
  }
  for (int i = array.size() - 1; i >= 0; --i) {
    sortedArray[countingArray[array[i]] - 1] = array[i];

    --countingArray[array[i]];
  }
  std::reverse(sortedArray.begin(), sortedArray.end());
  return sortedArray;
}

std::vector<int> sortedCounts(const std::vector<int>& array,
                              unsigned int maxKey) {
  std::vector<int> countingArray(maxKey + 1, 0);

  for (size_t i = 0; i < array.size(); ++i) {
    ++countingArray[array[i]];
  }

  return countingSortDesc(countingArray, array.size());
}

class Solution {
 public:
  int minSetSize(std::vector<int>& arr) {
    std::vector<int> arrGroupedByCount = sortedCounts(arr, MAX_I);
    int n = arr.size();
    int i = n;
    int ans = 0;
    while (i > (n / 2)) {
      i -= arrGroupedByCount[ans++];
    }
    return ans;
  }
};
