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
};