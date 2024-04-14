#include <string.h>

#include <iostream>

struct Pair {
 public:
  unsigned int key;
  char value[65];

  Pair() : key(0), value("") {}
};

class Container {
 private:
  size_t _size;
  size_t _capacity;
  Pair* _array;

 public:
  Container() noexcept : _size(0), _capacity(0), _array(nullptr) {}

  Container(const size_t& size) : _size(size), _capacity(size) {
    _array = new Pair[_capacity];
  }

  Container(const Container& other)
      : _size(other._size), _capacity(other._capacity) {
    _array = new Pair[_capacity];
    for (size_t i{0}; i < _size; ++i) {
      _array[i] = other._array[i];
    }
  }

  Container(Container&& other)
      : _size(other._size), _capacity(other._capacity) {
    _array = other._array;
    other._size = 0;
    other._capacity = 0;
    other._array = nullptr;
  }

  ~Container() noexcept { delete[] _array; }

  void reallocate(const size_t& newCapacity) {
    Pair* newArray = new Pair[newCapacity];
    for (size_t i{0}; i < _size; ++i) {
      newArray[i] = _array[i];
    }
    delete[] _array;
    _array = newArray;
    _capacity = newCapacity;
  }

  void pushBack(const Pair& pair) {
    if (_size >= _capacity) {
      reallocate((_size + 1) * 2);
    }
    _array[_size++] = pair;
  }

  size_t size() const noexcept { return _size; }

  Container& operator=(const Container& other) {
    _size = other._size;
    _capacity = other._capacity;
    delete[] _array;
    _array = new Pair[_capacity];
    for (size_t i{0}; i < _size; ++i) {
      _array[i] = other._array[i];
    }
    return *this;
  }

  Container& operator=(Container&& other) {
    _size = other._size;
    _capacity = other._capacity;
    _array = other._array;
    return *this;
  }

  Pair& operator[](const size_t& index) { return _array[index]; }

  const Pair& operator[](const size_t& index) const { return _array[index]; }

  void print(std::ostream& ostream) const noexcept {
    for (size_t i{0}; i < _size; ++i) {
      ostream << _array[i].key << "\t" << _array[i].value << std::endl;
    }
  }
};

std::ostream& operator<<(std::ostream& ostream, const Container& container) {
  container.print(ostream);
  return ostream;
}

void countingSort(const Container& array, Container& sortedArray,
                  unsigned int maxKey) {
  unsigned int countingArray[maxKey + 1];
  for (size_t i = 0; i <= maxKey; ++i) {
    countingArray[i] = 0;
  }
  for (size_t i = 0; i < array.size(); ++i) {
    ++countingArray[array[i].key];
  }
  for (size_t i = 1; i <= maxKey; ++i) {
    countingArray[i] += countingArray[i - 1];
  }
  for (int i = array.size() - 1; i >= 0; --i) {
    sortedArray[countingArray[array[i].key] - 1] = array[i];
    --countingArray[array[i].key];
  }
}

int main() {
  freopen("in.txt", "r", stdin);
  Container array;
  unsigned int maxKey{0};
  unsigned int key;
  char value[65];
  while (std::cin >> key) {
    std::cin >> value;
    Pair pair;
    pair.key = key;
    strcpy(pair.value, value);
    array.pushBack(pair);
    if (key > maxKey) maxKey = key;
  }
  Container sortedArray(array.size());
  countingSort(array, sortedArray, maxKey);
  // std::cout << sortedArray;
}