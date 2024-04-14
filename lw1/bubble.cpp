#include <iostream>
#include <vector>

using namespace std;

struct Pair {
 public:
  unsigned int key;
  char value[65];

  Pair() : key(0), value("") {}
};

int main() {
  freopen("in.txt", "r", stdin);
  vector<Pair> arr;

  Pair pair;
  while (cin >> pair.key) {
    cin >> pair.value;
    arr.push_back(pair);
  }

  Pair temp;
  for (int i = 0; i < arr.size() - 1; i++) {
    for (int j = 0; j < arr.size() - i - 1; j++) {
      if (arr[j].key > arr[j + 1].key) {
        temp = arr[j];
        arr[j] = arr[j + 1];
        arr[j + 1] = temp;
      }
    }
  }

  // for (Pair pair : arr) {
  //   cout << pair.key << "\t" << pair.value << endl;
  // }

  return 0;
}