#include <ctime>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

unsigned long seed = 1;  // это то самое стартовое значение,
                         // о котором мы говорили ранее

int my_rand(unsigned long long max) {
  seed = seed * 1103515245 + 12345;  // Линейный конгруэнтный метод
  return (seed / 65536) % max;  // Вернуть значение в диапазоне 0-32767
}

string rand_string() {
  string res = "";
  size_t length = 256;
  for (size_t i{0}; i < length; ++i) {
    res += (unsigned char)my_rand(93) + 33;
  }
  return res;
}

void rand_array(vector<string>& v) {
  size_t length = 50;
  for (size_t i{0}; i < length; ++i) {
    v.push_back(rand_string());
  }
}

std::string toLowerCase(const std::string& word) {
  std::string lowerCaseWord = "";
  for (size_t i{0}; i < word.size(); ++i) {
    lowerCaseWord += std::tolower(word[i]);
  }
  return lowerCaseWord;
}

std::string toUpperCase(const std::string& word) {
  std::string upperCaseWord = "";
  for (size_t i{0}; i < word.size(); ++i) {
    upperCaseWord += std::toupper(word[i]);
  }
  return upperCaseWord;
}

int main() {
  vector<string> v;
  rand_array(v);
  cout << "! save qwerty" << endl;

  for (string str : v) {
    cout << "+ " << str << " " << 18446744073709551615u << endl;
  }

  cout << "! save qwerty2" << endl;
  cout << "! load qwerty" << endl;

  size_t deleted = 0;
  vector<string> v_copy{v};
  cout << "! save qwerty" << endl;
  while (v.size() > 0) {
    size_t i = my_rand(v.size());
    // cout << v[i] << endl;
    cout << "- " << v[i] << endl;
    v[i] = v.back();
    v.pop_back();
    ++deleted;
  }

  cout << "! load qwerty2" << endl;

  for (size_t i{0}; i < v_copy.size(); ++i) {
    cout << toLowerCase(v_copy[i]) << endl;
  }

  for (size_t i{0}; i < v_copy.size(); ++i) {
    cout << "+ " << toLowerCase(v_copy[i]) << " " << 18446744073709551615u
         << endl;
  }

  cout << "! load qwerty" << endl;

  for (size_t i{0}; i < v_copy.size(); ++i) {
    cout << toLowerCase(v_copy[i]) << endl;
  }

  for (size_t i{0}; i < v_copy.size(); ++i) {
    cout << "+ " << toLowerCase(v_copy[i]) << " " << 18446744073709551615u
         << endl;
  }

  for (size_t i{0}; i < v_copy.size(); ++i) {
    cout << toLowerCase(v_copy[i]) << endl;
  }
}