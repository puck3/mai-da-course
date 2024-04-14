#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

struct Pair {
 public:
  unsigned int key;
  char value[65];

  Pair() : key(0), value("") {}
};

bool comparePairs(const Pair& a, const Pair& b) { return a.key < b.key; }

int main() {
  freopen("in.txt", "r", stdin);
  vector<Pair> arr;
  // заполнение массива
  Pair pair;
  while (cin >> pair.key) {
    cin >> pair.value;
    arr.push_back(pair);
  }
  sort(arr.begin(), arr.end(), comparePairs);

  // Вывод отсортированного массива на экран
  // for (Pair pair : arr) {
  //   cout << pair.key << "\t" << pair.value << endl;
  // }
  return 0;
}