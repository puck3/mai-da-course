#include <iostream>
#include <map>
#include <vector>

class SuffixTree {
 private:
  class Node {
   private:
    int start;
    int end;
    std::map<char, Node*> children;
    int leafNumber;

   public:
    Node(int start, int end = -1, int leafNumber = -1)
        : start(start), end(end), children(), leafNumber(leafNumber) {}

    ~Node() noexcept {
      for (auto& child : children) {
        delete child.second;
        child.second = nullptr;
      }
    }

    void addChild(int start, int end, char firstLetter, int leafNumber) {
      children[firstLetter] = new Node(start, end, leafNumber);
    }

    Node* split(int position, Node* parent, std::string_view text) {
      auto newInternalNode = new Node(start, start + position);
      parent->children[text[start]] = newInternalNode;

      start += position;
      newInternalNode->children[text[start]] = this;

      return newInternalNode;
    }

    int getStart() { return start; }

    int getEnd() { return end; }

    Node* getChild(char pathFirstLetter) {
      if (children.find(pathFirstLetter) != children.end()) {
        return children[pathFirstLetter];
      } else {
        return nullptr;
      }
    }

    void checkLeafs(std::vector<int>& leafsNumbers) {
      if (leafNumber < 0) {
        for (auto child : children) {
          child.second->checkLeafs(leafsNumbers);
        }
      } else if (leafsNumbers.empty()) {
        leafsNumbers.push_back(leafNumber);
      } else {
        auto it = leafsNumbers.begin();
        while (it != leafsNumbers.end() && *it < leafNumber) {
          ++it;
        }
        leafsNumbers.insert(it, leafNumber);
      }
    }
  };

  Node* root;
  std::string_view text;

 public:
  SuffixTree(std::string_view text) : root(new Node{-1}), text(text) {
    for (int i = 0; i < text.size(); ++i) {
      addSuffix(i);
    }
  }

  ~SuffixTree() noexcept { delete root; }

  void addSuffix(int suffixNumber) {
    Node* currentNode = root;
    int currentLetter = suffixNumber;
    while (true) {
      Node* nextNode = currentNode->getChild(text[currentLetter]);
      if (!nextNode) {
        currentNode->addChild(currentLetter, text.size(), text[currentLetter],
                              suffixNumber);
        return;
      }
      int start = nextNode->getStart();
      int end = nextNode->getEnd();
      for (int i = start; i < end; ++i) {
        if (text[currentLetter] != text[i]) {
          nextNode = nextNode->split(i - start, currentNode, text);
          nextNode->addChild(currentLetter, text.size(), text[currentLetter],
                             suffixNumber);
          return;
        }
        ++currentLetter;
      }
      currentNode = nextNode;
    }
  }

  std::vector<int> find(std::string_view pattern) {
    Node* currentNode = root;
    auto currentLetter = pattern.begin();
    while (true) {
      currentNode = currentNode->getChild(*currentLetter);
      if (!currentNode) {
        return std::vector<int>{};
      }
      int start = currentNode->getStart();
      int end = currentNode->getEnd();
      for (int i = start; i < end; ++i) {
        if (*currentLetter != text[i]) {
          return std::vector<int>{};
        }
        if (++currentLetter == pattern.end()) {
          std::vector<int> leafsNumbers;
          currentNode->checkLeafs(leafsNumbers);
          return leafsNumbers;
        }
      }
    }
  }
};

int main() {
  std::string text;
  std::cin >> text;
  text += "$";
  SuffixTree suffixTree{text};
  std::string pattern;
  int i = 1;
  while (std::cin >> pattern) {
    std::vector<int> patternEntries = suffixTree.find(pattern);
    if (!patternEntries.empty()) {
      std::cout << i << ": ";
      for (int j = 0; j < patternEntries.size(); ++j) {
        if (j == 0) {
          std::cout << patternEntries[j] + 1;
        } else {
          std::cout << ", " << patternEntries[j] + 1;
        }
      }
      std::cout << "\n";
    }
    ++i;
  }
}