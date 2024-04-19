#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

enum colorType { red, black };

struct Node {
  colorType color;
  std::string key;
  u_int64_t data;
  Node* left;
  Node* right;
  Node* parent;

  Node()
      : color(black),
        key(std::string()),
        data(u_int64_t()),
        left(nullptr),
        right(nullptr),
        parent(nullptr) {}

  Node(colorType colorValue, const std::string& keyValue,
       const u_int64_t& dataValue, Node* leftChild, Node* rightChild,
       Node* parentNode)
      : color(colorValue),
        key(keyValue),
        data(dataValue),
        left(leftChild),
        right(rightChild),
        parent(parentNode) {}
};

class RedBlackTree {
 private:
  Node* nullNode;
  Node* root;
  size_t elementsCount;

  void leftRotate(Node* oldRoot) {
    if (isNull(oldRoot) || isNull(oldRoot->right)) {
      return;
    }
    Node* newRoot = oldRoot->right;
    oldRoot->right = newRoot->left;
    if (!isNull(newRoot->left)) {
      newRoot->left->parent = oldRoot;
    }
    newRoot->parent = oldRoot->parent;
    if (isNull(oldRoot->parent)) {
      this->root = newRoot;
    } else if (isLeftSon(oldRoot)) {
      oldRoot->parent->left = newRoot;
    } else {
      oldRoot->parent->right = newRoot;
    }
    newRoot->left = oldRoot;
    oldRoot->parent = newRoot;
  }

  void rightRotate(Node* oldRoot) {
    if (isNull(oldRoot) || isNull(oldRoot->left)) {
      return;
    }
    Node* newRoot = oldRoot->left;
    oldRoot->left = newRoot->right;
    if (!isNull(newRoot->right)) {
      newRoot->right->parent = oldRoot;
    }
    newRoot->parent = oldRoot->parent;
    if (isNull(oldRoot->parent)) {
      this->root = newRoot;
    } else if (isLeftSon(oldRoot)) {
      oldRoot->parent->left = newRoot;
    } else {
      oldRoot->parent->right = newRoot;
    }
    newRoot->right = oldRoot;
    oldRoot->parent = newRoot;
  }

  void insertFixup(Node* brokenNode) {
    while (!isNull(brokenNode->parent) && brokenNode->parent->color != black) {
      Node* uncle = isLeftSon(brokenNode->parent)
                        ? brokenNode->parent->parent->right
                        : brokenNode->parent->parent->left;
      if (uncle->color == red) {
        brokenNode->parent->color = black;
        uncle->color = black;
        brokenNode->parent->parent->color = red;
        brokenNode = brokenNode->parent->parent;
      } else {
        if (isLeftSon(brokenNode->parent) && isRightSon(brokenNode)) {
          brokenNode = brokenNode->parent;
          leftRotate(brokenNode);
        } else if (isRightSon(brokenNode->parent) && isLeftSon(brokenNode)) {
          brokenNode = brokenNode->parent;
          rightRotate(brokenNode);
        }

        brokenNode->parent->color = black;
        brokenNode->parent->parent->color = red;
        if (isLeftSon(brokenNode->parent)) {
          rightRotate(brokenNode->parent->parent);
        } else {
          leftRotate(brokenNode->parent->parent);
        }
      }
    }
    root->color = black;
  }

  void replace(Node* oldSubtree, Node* newSubtree) {
    if (isNull(oldSubtree)) {
      return;
    }
    if (isNull(oldSubtree->parent)) {
      root = newSubtree;
    } else if (isLeftSon(oldSubtree)) {
      oldSubtree->parent->left = newSubtree;
    } else {
      oldSubtree->parent->right = newSubtree;
    }
    if (!isNull(newSubtree)) {
      newSubtree->parent = oldSubtree->parent;
    }
  }

  void eraseFixup(Node* brokenNode, Node* parent) {
    while (brokenNode != root && brokenNode->color == black) {
      Node* brother =
          isLeftSon(brokenNode, parent) ? parent->right : parent->left;
      if (brother->color == red) {
        brother->color = black;
        parent->color = red;
        if (isLeftSon(brokenNode, parent)) {
          leftRotate(parent);
          brother = parent->right;
        } else {
          rightRotate(parent);
          brother = parent->left;
        }
      }
      if (brother->left->color == black && brother->right->color == black) {
        brother->color = red;
        brokenNode = parent;
        parent = parent->parent;
      } else {
        if (isLeftSon(brokenNode, parent)) {
          if (brother->right->color == black) {
            brother->left->color = black;
            brother->color = red;
            rightRotate(brother);
            brother = parent->right;
          }
          brother->color = parent->color;
          parent->color = black;
          brother->right->color = black;
          leftRotate(parent);
        } else {
          if (brother->left->color == black) {
            brother->right->color = black;
            brother->color = red;
            leftRotate(brother);
            brother = parent->left;
          }
          brother->color = parent->color;
          parent->color = black;
          brother->left->color = black;
          rightRotate(parent);
        }
        brokenNode = root;
        parent = nullNode;
      }
    }
    brokenNode->color = black;
    // printTree();
  }

  void deleteSubtree(Node* root) {
    if (isNull(root)) {
      return;
    }
    if (!isNull(root->left)) {
      deleteSubtree(root->left);
    }
    if (!isNull(root->right)) {
      deleteSubtree(root->right);
    }
    if (isNull(root->parent)) {
      this->root = nullNode;
    } else if (isLeftSon(root)) {
      root->parent->left = nullNode;
    } else {
      root->parent->right = nullNode;
    }
    delete root;
    root = nullptr;
  }

  void copy(Node*& thisNode, const Node* otherNode,
            const RedBlackTree& otherTree) {
    if (otherTree.isNull(otherNode)) {
      if (!isNull(thisNode)) {
        deleteSubtree(thisNode->left);
        deleteSubtree(thisNode->right);
      }
      thisNode = nullNode;
      return;
    }
    if (isNull(thisNode)) {
      thisNode = new Node();
    }
    thisNode->parent = nullNode;
    thisNode->color = otherNode->color;
    thisNode->key = otherNode->key;
    thisNode->data = otherNode->data;
    copy(thisNode->left, otherNode->left, otherTree);
    if (otherNode->left != otherTree.nullNode) {
      thisNode->left->parent = thisNode;
    }
    copy(thisNode->right, otherNode->right, otherTree);
    if (otherNode->right != otherTree.nullNode) {
      thisNode->right->parent = thisNode;
    }
  }

  void saveNodes(std::ofstream& outputFile, const Node* node) const {
    if (isNull(node)) {
      return;
    }
    size_t length = node->key.size();
    outputFile.write(reinterpret_cast<char*>(&length), sizeof(size_t));
    outputFile.write(node->key.c_str(), length);
    // const u_int64_t& data = node->data;
    outputFile.write(reinterpret_cast<const char*>(&node->data),
                     sizeof(u_int64_t));
    saveNodes(outputFile, node->left);
    saveNodes(outputFile, node->right);
  }

 public:
  RedBlackTree() {
    nullNode = new Node();
    root = nullNode;
    root->parent = nullNode;
    elementsCount = 0;
  }

  RedBlackTree(const RedBlackTree& other) : RedBlackTree() {
    copy(root, other.root, other);
    elementsCount = other.elementsCount;
  }

  RedBlackTree(RedBlackTree&& other)
      : nullNode(other.nullNode),
        root(other.root),
        elementsCount(other.elementsCount) {
    other.nullNode = other.root = nullptr;
    other.elementsCount = 0;
  }

  virtual ~RedBlackTree() {
    deleteSubtree(root);
    delete nullNode;
    nullNode = root = nullptr;
  }

  RedBlackTree& operator=(const RedBlackTree& other) {
    copy(root, other.root, other);
    elementsCount = other.elementsCount;
    return *this;
  }

  RedBlackTree& operator=(RedBlackTree&& other) {
    deleteSubtree(root);
    delete nullNode;
    nullNode = other.nullNode;
    root = other.root;
    elementsCount = other.elementsCount;
    other.nullNode = other.root = nullptr;
    other.elementsCount = 0;
    return *this;
  }

  Node* find(const std::string& key) const {
    Node* foundNode = root;
    while (!isNull(foundNode) && foundNode->key != key) {
      foundNode = key < foundNode->key ? foundNode->left : foundNode->right;
    }
    return foundNode;
  }

  void insert(const std::string& key, const u_int64_t& data) {
    Node* parent = nullNode;
    Node* child = root;
    while (!isNull(child)) {
      parent = child;
      if (key < child->key) {
        child = child->left;
      } else if (key > child->key) {
        child = child->right;
      } else {
        throw std::runtime_error("Exist");
      }
    }
    Node* newChild = new Node(red, key, data, nullNode, nullNode, parent);
    if (isNull(parent)) {
      root = newChild;
    } else if (key < parent->key) {
      parent->left = newChild;
    } else {
      parent->right = newChild;
    }
    insertFixup(newChild);
    ++elementsCount;
  }

  Node* min(Node* root) {
    if (isNull(root)) {
      return root;
    }
    while (!isNull(root->left)) {
      root = root->left;
    }
    return root;
  }

  Node* max(Node* root) {
    if (isNull(root)) {
      return root;
    }
    while (!isNull(root->right)) {
      root = root->right;
    }
    return root;
  }

  void erase(Node* target) {
    colorType erasedColor = target->color;
    Node *movedNode, *parent;
    if (isNull(target->left)) {
      movedNode = target->right;
      parent = target->parent;
      replace(target, target->right);
    } else if (isNull(target->right)) {
      movedNode = target->left;
      parent = target->parent;
      replace(target, target->left);
    } else {
      Node* rightSubtreeMinimum = min(target->right);
      erasedColor = rightSubtreeMinimum->color;
      movedNode = rightSubtreeMinimum->right;
      parent = rightSubtreeMinimum;
      if (rightSubtreeMinimum->parent != target) {
        parent = rightSubtreeMinimum->parent;
        replace(rightSubtreeMinimum, rightSubtreeMinimum->right);
        rightSubtreeMinimum->right = target->right;
        rightSubtreeMinimum->right->parent = rightSubtreeMinimum;
      }
      replace(target, rightSubtreeMinimum);
      rightSubtreeMinimum->left = target->left;
      rightSubtreeMinimum->left->parent = rightSubtreeMinimum;
      rightSubtreeMinimum->color = target->color;
    }
    if (erasedColor == black) {
      eraseFixup(movedNode, parent);
    }
    delete target;
    --elementsCount;
  }

  void clear() {
    deleteSubtree(root);
    elementsCount = 0;
  }

  bool isNull(const Node* checkedNode) const {
    return checkedNode == nullptr || checkedNode == nullNode;
  }

  bool isLeftSon(const Node* child, const Node* parent = nullptr) const {
    if (!parent) {
      parent = child->parent;
    }
    return !isNull(parent) ? child == parent->left : false;
  }

  bool isRightSon(const Node* child, const Node* parent = nullptr) const {
    if (!parent) {
      parent = child->parent;
    }
    return !isNull(parent) ? child == parent->right : false;
  }

  void save(std::ofstream& outputFile) {
    outputFile.write(reinterpret_cast<char*>(&elementsCount), sizeof(size_t));

    saveNodes(outputFile, root);
  }

  void load(std::ifstream& inputFile) {
    clear();
    size_t count;
    inputFile.read(reinterpret_cast<char*>(&count), sizeof(size_t));
    size_t keyLength;
    char* keyBuffer;
    u_int64_t data;
    for (size_t i{0}; i < count; ++i) {
      inputFile.read(reinterpret_cast<char*>(&keyLength), sizeof(size_t));
      keyBuffer = new char[keyLength];
      inputFile.read(keyBuffer, keyLength);
      inputFile.read(reinterpret_cast<char*>(&data), sizeof(u_int64_t));
      insert(std::string(keyBuffer, keyLength), data);
      delete[] keyBuffer;
      keyBuffer = nullptr;
    }
  }

  void printTree(size_t depth = 0, Node* node = nullptr) {
    if (!node) {
      node = root;
      std::cout << "new tree" << std::endl;
    }
    if (isNull(node)) {
      std::cout << std::endl;
      return;
    }
    std::cout << std::endl;
    printTree(depth + 1, node->right);
    for (size_t i{0}; i < 2 * depth; ++i) {
      std::cout << "\t";
    }
    if (node->color == red) {
      std::cout << "red: ";
    } else {
      std::cout << "black: ";
    }
    // std::cout << node->key;
    std::cout << std::endl;
    printTree(depth + 1, node->left);
    std::cout << std::endl;
  }
};

std::string& toLowerCase(std::string& word) {
  for (size_t i{0}; i < word.size(); ++i) {
    word[i] = std::tolower(word[i]);
  }
  return word;
}

class Dictionary {
 private:
  RedBlackTree tree;

 public:
  Dictionary() = default;

  void insert(std::string& key, const u_int64_t& value) {
    try {
      tree.insert(toLowerCase(key), value);
      // std::cout << "inserted ";
      std::cout << "OK" << std::endl;
    } catch (std::runtime_error& error) {
      std::cout << error.what() << std::endl;
    } catch (std::exception& error) {
      std::cout << "ERROR: " << error.what() << std::endl;
    }
  }

  void at(std::string& key) {
    try {
      Node* foundNode = tree.find(toLowerCase(key));
      if (!tree.isNull(foundNode)) {
        // std::cout << "found ";
        std::cout << "OK: " << foundNode->data << std::endl;
      } else {
        std::cout << "NoSuchWord" << std::endl;
      }
    } catch (std::exception& error) {
      std::cout << "ERROR: " << error.what() << std::endl;
    }
  }

  void erase(std::string& key) {
    try {
      Node* foundNode = tree.find(toLowerCase(key));
      if (!tree.isNull(foundNode)) {
        tree.erase(foundNode);
        // std::cout << "erased ";
        std::cout << "OK" << std::endl;
      } else {
        std::cout << "NoSuchWord" << std::endl;
      }
    } catch (std::exception& error) {
      std::cout << "ERROR: " << error.what() << std::endl;
    }
  }

  void save(std::string& path) {
    std::ofstream outputFile;
    try {
      outputFile.open(path, std::ios::trunc | std::ios::out | std::ios::binary);
      tree.save(outputFile);
      // std::cout << "saved ";
      std::cout << "OK" << std::endl;
    } catch (std::exception& error) {
      std::cout << "ERROR: " << error.what() << std::endl;
    }
    outputFile.close();
  }

  void load(std::string& path) {
    std::ifstream inputFile;
    try {
      inputFile.open(path, std::ios::binary | std::ios::in);
      tree.load(inputFile);
      // std::cout << "loaded ";
      std::cout << "OK" << std::endl;
    } catch (std::exception& error) {
      std::cout << "ERROR: " << error.what() << std::endl;
    }
    inputFile.close();
  }

  void print() { tree.printTree(); }
};

int main() {
  freopen("input.txt", "r", stdin);
  Dictionary dictionary;
  std::string input;
  std::string line;
  while (std::getline(std::cin, line)) {
    std::cin.clear();
    std::istringstream read{line};
    read >> input;
    if (input == "+") {
      std::string word;
      u_int64_t number;
      read >> word >> number;
      dictionary.insert(word, number);
    } else if (input == "-") {
      std::string word;
      read >> word;
      dictionary.erase(word);
    } else if (input == "!") {
      std::string operation, path;
      read >> operation >> path;
      if (operation == "save") {
        dictionary.save(path);
      } else {
        dictionary.load(path);
      }
      // } else if (input == "#") {
      //   dictionary.print();
    } else {
      dictionary.at(input);
    }
  }
}
