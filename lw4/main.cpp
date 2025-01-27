#include <vector>
#include <string>
#include <iostream>

void evaluateZ(const std::string &text, std::vector<int> &z) {
  int left = 0, right = 0;
  for (int i = 1; i < text.length(); ++i) {
    if (right > i) {
      z[i] = std::min(z[i - left], right - i);
    }
    while (i + z[i] < text.length() && text[z[i]] == text[i + z[i]]) {
      ++z[i];
    }
    if ((i + z[i]) > right) {
      left = i;
      right = i + z[i];
    }
  }
}

int main() {
  std::string text, pattern;
  std::cin >> text >> pattern;
  std::string processingText = pattern + "$" + text;
  std::vector<int> z(processingText.length(), 0);
  evaluateZ(processingText, z);
  for (size_t i = 0; i < z.size(); ++i) {
    if (z[i] == pattern.length()) {
      std::cout << i - (pattern.size() + 1) << std::endl;
    }
  }
}