#include <cmath>
#include <iostream>
#include <map>
#include <vector>

std::pair<std::vector<size_t>, size_t> compress(const std::string& text) {
  std::map<std::string, size_t> dictionary;
  size_t next_code = 0;
  for (char c = 'a'; c <= 'z'; ++c) {
    dictionary[std::string(1, c)] = next_code++;
  }
  dictionary["EOF"] = next_code++;

  std::vector<size_t> codes;
  std::string substring;
  size_t max_code = 27;
  for (char c : text) {
    if (dictionary.find(substring + c) == dictionary.end()) {
      codes.push_back(dictionary[substring]);
      max_code =
          dictionary[substring] > max_code ? dictionary[substring] : max_code;
      dictionary[substring + c] = next_code++;
      substring = std::string(1, c);
    } else {
      substring += c;
    }
  }
  if (!substring.empty()) {
    codes.push_back(dictionary[substring]);
    max_code =
        dictionary[substring] > max_code ? dictionary[substring] : max_code;
  }
  codes.push_back(dictionary["EOF"]);
  return std::pair(codes, max_code + 1);
}

std::string decompress(const std::vector<size_t>& codes) {
  std::vector<std::string> dictionary;
  for (char c = 'a'; c <= 'z'; ++c) {
    dictionary.push_back(std::string(1, c));
  }
  dictionary.push_back("EOF");

  std::string text;
  for (size_t code : codes) {
    if (dictionary[code] == "EOF") {
      break;
    }
    if (dictionary.size() != 27) {
      dictionary.back() += dictionary[code].front();
    }
    text += dictionary[code];
    dictionary.push_back(dictionary[code]);
  }
  return text;
}

int main() {
  std::string action;
  std::cin >> action;
  if (action == "compress") {
    std::string text;
    std::cin >> text;
    auto [codes, codes_count] = compress(text);
    for (size_t code : codes) {
      std::cout << code << " ";
    }
    std::cout << std::endl;
    // std::cout << "Количество символов: " << text.size() << std::endl;
    // std::cout << "Количество уникальных кодов в сообщении: " << codes_count
    //           << std::endl;
    // std::cout << "Количество кодов: " << codes.size() << std::endl;
    // double bits = std::ceil(std::log2(codes_count));
    // std::cout << "Минимальное количество бит для хранения одного кода: " <<
    // bits
    //           << std::endl;
    // double size = ((codes.size() * bits) / (text.size() * 5.0)) * 100.0;
    // std::cout << "Размер закодированных данных, относительно исходных: " <<
    // size
    //           << "%" << std::endl;
    // std::cout << "Степень сжатия данных: " << 100.0 - size << "%" <<
    // std::endl;
  } else if (action == "decompress") {
    std::vector<size_t> codes;
    size_t code;
    while (std::cin >> code) {
      codes.push_back(code);
    }
    std::string text = decompress(codes);
    std::cout << text << std::endl;
  }
}
