#include <algorithm>
#include <iostream>
#include <map>
#include <vector>

class SuffixTree {
 private:
  class Node {
   private:
    std::string path;
    std::map<char, Node*> children;
    int leafNumber;

   public:
    Node(const std::string& path = "", int leafNumber = -1)
        : path(path), children(), leafNumber(leafNumber) {}

    Node(std::string&& path, int leafNumber)
        : path(path), children(), leafNumber(leafNumber) {}

    ~Node() noexcept {
      for (auto& child : children) {
        delete child.second;
        child.second = nullptr;
      }
    }

    void addChild(const std::string& leafPath, int leafNumber) {
      char pathFirstLetter = leafPath[0];
      children[pathFirstLetter] = new Node(leafPath, leafNumber);
    }

    void addChild(std::string&& leafPath, int leafNumber) {
      char pathFirstLetter = leafPath[0];
      children[pathFirstLetter] = new Node(leafPath, leafNumber);
    }

    Node* split(int position, Node* parent) {
      auto newInternalNode = new Node(path.substr(0, position));
      parent->children[path[0]] = newInternalNode;
      path = path.substr(position);
      newInternalNode->children[path[0]] = this;

      return newInternalNode;
    }

    const std::string& getPath() { return path; }

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

    bool isLeaf() { return leafNumber != -1; }
  };

  Node* root;

 public:
  SuffixTree(std::string text) : root(new Node{}) {
    text += "$";
    for (int i = 0; i < text.size(); ++i) {
      addSuffix(text.substr(i), i);
    }
  }

  SuffixTree(std::vector<std::string> words) : root(new Node{}) {
    for (int i = 0; i < words.size(); ++i) {
      words[i] += "$";
      addSuffix(words[i], i);
    }
  }

  ~SuffixTree() noexcept { delete root; }

  void addSuffix(const std::string& suffix, int suffixNumber) {
    Node* currentNode = root;
    auto currentLetter = suffix.begin();
    while (true) {
      Node* nextNode = currentNode->getChild(*currentLetter);
      if (!nextNode) {
        std::string path{currentLetter, suffix.end()};
        currentNode->addChild(std::move(path), suffixNumber);
        return;
      }
      const std::string& nextNodePath = nextNode->getPath();
      for (int i = 0; i < nextNodePath.size(); ++i) {
        if (*currentLetter != nextNodePath[i]) {
          nextNode = nextNode->split(i, currentNode);
          std::string path{currentLetter, suffix.end()};
          nextNode->addChild(path, suffixNumber);
          return;
        }
        ++currentLetter;
      }
      currentNode = nextNode;
    }
  }

  std::vector<int> find(const std::string& pattern) {
    Node* currentNode = root;
    auto currentLetter = pattern.begin();
    while (true) {
      currentNode = currentNode->getChild(*currentLetter);
      if (!currentNode) {
        return std::vector<int>{};
      }
      const std::string& path = currentNode->getPath();
      for (int i = 0; i < path.size(); ++i) {
        if (*currentLetter != path[i]) {
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

  bool checkWord(const std::string& word) {
    Node* currentNode = root;
    for (auto currentLetter = word.begin(); currentLetter != word.end();
         ++currentLetter) {
      currentNode = currentNode->getChild(*currentLetter);
      if (!currentNode ||
          (!currentNode->isLeaf() && currentNode->getPath().size() != 1)) {
        return false;
      }
    }
    return true;
  }
};

bool cmp(const std::string& a, const std::string& b) {
  if (a.size() == b.size()) return a > b;
  return a.size() < b.size();
}

int main() {
  std::string word;
  std::vector<std::string> words;
  while (std::cin >> word) {
    words.push_back(word);
  }
  std::sort(words.begin(), words.end(), cmp);

  SuffixTree suffixTree(words);
  std::string ans;
  for (auto word : words) {
    std::cout << word << std::endl;
    if (suffixTree.checkWord(word)) {
      ans = word;
    }
  }
  std::cout << ans << std::endl;
}