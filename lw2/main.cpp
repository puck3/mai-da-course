#include <fstream>
#include <iostream>
#include <string>

enum colorType { red, black };

template <typename Key, typename T>
struct Node {
  colorType color;
  Key key;
  T data;
  Node<Key, T>* left;
  Node<Key, T>* right;
  Node<Key, T>* parent;

  Node() noexcept
      : color(black),
        key(Key()),
        data(T()),
        left(nullptr),
        right(nullptr),
        parent(nullptr) {}

  Node(colorType colorValue, const Key& keyValue, const T& dataValue,
       Node<Key, T>* leftChild, Node<Key, T>* rightChild,
       Node<Key, T>* parentNode) noexcept
      : color(colorValue),
        key(keyValue),
        data(dataValue),
        left(leftChild),
        right(rightChild),
        parent(parentNode) {}

  bool isLeftSon() const { return this == this->parent->left; }

  bool isRightSon() const { return this == this->parent->right; }
};

// using T = unsigned long long;
// using Key = std::string;

template <typename Key, typename T>
class RedBlackTree {
 private:
  Node<Key, T>* nullNode;
  Node<Key, T>* root;
  size_t elementsCount;

  void leftRotate(Node<Key, T>* oldRoot) {
    if (isNull(oldRoot->right)) {
      throw std::runtime_error("Unable to do left rotate");
    }
    Node<Key, T>* newRoot = oldRoot->right;
    oldRoot->right = newRoot->left;
    if (!isNull(newRoot->left)) {
      newRoot->left->parent = oldRoot;
    }
    newRoot->parent = oldRoot->parent;
    if (isNull(oldRoot->parent)) {
      this->root = newRoot;
    } else if (oldRoot->isLeftSon()) {
      oldRoot->parent->left = newRoot;
    } else {
      oldRoot->parent->right = newRoot;
    }
    newRoot->left = oldRoot;
    oldRoot->parent = newRoot;
  }

  void rightRotate(Node<Key, T>* oldRoot) {
    if (isNull(oldRoot->left)) {
      throw std::runtime_error("Unable to do right rotate");
    }
    Node<Key, T>* newRoot = oldRoot->left;
    oldRoot->left = newRoot->right;
    if (!isNull(newRoot->right)) {
      newRoot->right->parent = oldRoot;
    }
    newRoot->parent = oldRoot->parent;
    if (isNull(oldRoot->parent)) {
      this->root = newRoot;
    } else if (oldRoot->isLeftSon()) {
      oldRoot->parent->left = newRoot;
    } else {
      oldRoot->parent->right = newRoot;
    }
    newRoot->right = oldRoot;
    oldRoot->parent = newRoot;
  }

  void insertFixup(Node<Key, T>* brokenNode) {
    while (brokenNode->parent->color != black) {
      Node<Key, T>* uncle = brokenNode->parent->isLeftSon()
                                ? brokenNode->parent->parent->right
                                : brokenNode->parent->parent->left;
      if (uncle->color == red) {
        brokenNode->parent->color = black;
        uncle->color = black;
        brokenNode->parent->parent->color = red;
        brokenNode = brokenNode->parent->parent;
      } else {
        if (brokenNode->parent->isLeftSon() && brokenNode->isRightSon()) {
          brokenNode = brokenNode->parent;
          leftRotate(brokenNode);
        } else if (brokenNode->parent->isRightSon() &&
                   brokenNode->isLeftSon()) {
          brokenNode = brokenNode->parent;
          rightRotate(brokenNode);
        }

        brokenNode->parent->color = black;
        brokenNode->parent->parent->color = red;
        if (brokenNode->parent->isLeftSon()) {
          rightRotate(brokenNode->parent->parent);
        } else {
          leftRotate(brokenNode->parent->parent);
        }
      }
    }
    root->color = black;
  }

  void replace(Node<Key, T>* oldSubtree, Node<Key, T>* newSubtree) {
    if (isNull(oldSubtree->parent)) {
      root = newSubtree;
    } else if (oldSubtree->isLeftSon()) {
      oldSubtree->parent->left = newSubtree;
    } else {
      oldSubtree->parent->right = newSubtree;
    }
    if (!isNull(newSubtree)) {
      newSubtree->parent = oldSubtree->parent;
    }
  }

  void eraseFixup(Node<Key, T>* brokenNode) {
    while (brokenNode != root && brokenNode->color != black) {
      Node<Key, T>* brother = brokenNode->isLeftSon()
                                  ? brokenNode->parent->right
                                  : brokenNode->parent->left;
      if (brother->color == red) {
        brother->color = black;
        brokenNode->parent->color = red;
        if (brokenNode->isLeftSon()) {
          leftRotate(brokenNode->parent);
          brother = brokenNode->parent->right;
        } else {
          rightRotate(brokenNode->parent);
          brother = brokenNode->parent->left;
        }
      }
      if (brother->left->color == black && brother->right->color == black) {
        brother->color = red;
        brokenNode = brokenNode->parent;
      } else {
        if (brokenNode->isLeftSon()) {
          if (brother->right->color == black) {
            brother->left->color = black;
            brother->color = red;
            rightRotate(brother);
            brother = brokenNode->parent->right;
          }
          brother->color = brokenNode->parent->color;
          brokenNode->parent->color = black;
          brother->left->color = black;
          leftRotate(brokenNode->parent);
        } else {
          if (brokenNode->isRightSon() && brother->left->color == black) {
            brother->right->color = black;
            brother->color = red;
            leftRotate(brother);
            brother = brokenNode->parent->left;
          }
          brother->color = brokenNode->parent->color;
          brokenNode->parent->color = black;
          brother->right->color = black;
          rightRotate(brokenNode->parent);
        }
        brokenNode = root;
      }
    }
    brokenNode->color = black;
  }

  void deleteSubtree(Node<Key, T>* root) noexcept {
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
    } else if (root->isLeftSon()) {
      root->parent->left = nullNode;
    } else {
      root->parent->right = nullNode;
    }
    delete root;
    root = nullptr;
  }

  void copy(Node<Key, T>*& thisNode, const Node<Key, T>* otherNode,
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
      thisNode = new Node<Key, T>();
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

 public:
  RedBlackTree() {
    nullNode = new Node<Key, T>();
    root = nullNode;
    root->parent = nullNode;
    elementsCount = 0;
  }

  RedBlackTree(const RedBlackTree& other) : RedBlackTree() {
    copy(root, other.root, other);
    elementsCount = other.elementsCount;
  }

  RedBlackTree(RedBlackTree&& other) noexcept
      : nullNode(other.nullNode),
        root(other.root),
        elementsCount(other.elementsCount) {
    other.nullNode = other.root = nullptr;
    other.elementsCount = 0;
  }

  virtual ~RedBlackTree() noexcept {
    deleteSubtree(root);
    delete nullNode;
    nullNode = root = nullptr;
  }

  RedBlackTree& operator=(const RedBlackTree& other) {
    copy(root, other.root, other);
    elementsCount = other.elementsCount;
    return *this;
  }

  RedBlackTree& operator=(RedBlackTree&& other) noexcept {
    deleteSubtree(root);
    delete nullNode;
    nullNode = other.nullNode;
    root = other.root;
    elementsCount = other.elementsCount;
    other.nullNode = other.root = nullptr;
    other.elementsCount = 0;
    return *this;
  }

  Node<Key, T>* find(const Key& key) const noexcept {
    Node<Key, T>* foundNode = root;
    while (!isNull(foundNode) && foundNode->key != key) {
      foundNode = key < foundNode->key ? foundNode->left : foundNode->right;
    }
    return foundNode;
  }

  void insert(const Key& key, const T& data) {
    Node<Key, T>* parent = nullNode;
    Node<Key, T>* child = root;
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
    Node<Key, T>* newChild =
        new Node<Key, T>(red, key, data, nullNode, nullNode, parent);
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

  Node<Key, T>* min(Node<Key, T>* root = nullptr) noexcept {
    if (root == nullptr) {
      root = this->root;
    }
    if (root == nullNode) {
      return root;
    }
    while (!isNull(root->left)) {
      root = root->left;
    }
    return root;
  }

  Node<Key, T>* max(Node<Key, T>* root = nullptr) noexcept {
    if (root == nullptr) {
      root = this->root;
    }
    if (root == nullNode) {
      return root;
    }
    while (!isNull(root->right)) {
      root = root->right;
    }
    return root;
  }

  void erase(Node<Key, T>* target) {
    colorType erasedColor = target->color;
    Node<Key, T>* movedNode;
    if (isNull(target->left)) {
      movedNode = target->right;
      replace(target, target->right);
    } else if (isNull(target->right)) {
      movedNode = target->left;
      replace(target, target->left);
    } else {
      Node<Key, T>* rightSubtreeMinimum = min(target->right);
      erasedColor = rightSubtreeMinimum->color;
      movedNode = rightSubtreeMinimum->right;
      if (rightSubtreeMinimum->parent != target) {
        replace(rightSubtreeMinimum, rightSubtreeMinimum->right);
        rightSubtreeMinimum->right = target->right;
        rightSubtreeMinimum->right->parent = rightSubtreeMinimum;
      }
      replace(target, rightSubtreeMinimum);
      rightSubtreeMinimum->left = target->left;
      rightSubtreeMinimum->left->parent = rightSubtreeMinimum;
      rightSubtreeMinimum->color = target->color;
    }
    delete target;
    if (erasedColor == black) {
      eraseFixup(movedNode);
    }
    --elementsCount;
  }

  void clear() noexcept {
    deleteSubtree(root);
    elementsCount = 0;
  }

  bool isNull(const Node<Key, T>* checkedNode) const noexcept {
    return checkedNode == nullptr || checkedNode == nullNode;
  }

  void saveNodes(std::ofstream& outputFile, const Node<Key, T>* node) const {
    if (isNull(node)) {
      return;
    }
    outputFile.write(reinterpret_cast<const char*>(&node->key), sizeof(Key));
    outputFile.write(reinterpret_cast<const char*>(&node->data), sizeof(T));
    saveNodes(outputFile, node->left);
    saveNodes(outputFile, node->right);
  }

  void save(std::ofstream& outputFile) {
    outputFile.write(reinterpret_cast<const char*>(&elementsCount),
                     sizeof(size_t));

    saveNodes(outputFile, root);
  }

  void load(std::ifstream& inputFile) {
    clear();
    size_t count{0};
    inputFile.read(reinterpret_cast<char*>(&count), sizeof(size_t));
    Key key;
    T data;
    for (size_t i{0}; i < count;) {
      inputFile.read(reinterpret_cast<char*>(&key), sizeof(Key));
      inputFile.read(reinterpret_cast<char*>(&data), sizeof(T));
      insert(key, data);
    }
  }
};

std::string toLowerCase(const std::string& word) {
  std::string lowerCaseWord = "";
  for (char letter : word) {
    lowerCaseWord += std::tolower(letter);
  }
  return lowerCaseWord;
}

class Dictionary {
 private:
  RedBlackTree<std::string, unsigned long long> tree;

 public:
  Dictionary() = default;

  void insert(const std::string& key, const unsigned long long& value) {
    try {
      tree.insert(toLowerCase(key), value);
      std::cout << "OK" << std::endl;
    } catch (std::runtime_error& error) {
      std::cout << error.what() << std::endl;
    }
  }

  void at(const std::string& key) {
    Node<std::string, unsigned long long>* foundNode =
        tree.find(toLowerCase(key));
    if (!tree.isNull(foundNode)) {
      std::cout << "OK: " << foundNode->data << std::endl;
    } else {
      std::cout << "NoSuchWord" << std::endl;
    }
  }

  void erase(const std::string& key) {
    Node<std::string, unsigned long long>* foundNode =
        tree.find(toLowerCase(key));
    if (!tree.isNull(foundNode)) {
      tree.erase(foundNode);
      std::cout << "OK" << std::endl;
    } else {
      std::cout << "NoSuchWord" << std::endl;
    }
  }

  void save(std::string& path) {
    std::ofstream outputFile;
    outputFile.open(path, std::ios::binary);
    tree.save(outputFile);
  }

  void load(std::string& path) {
    std::ifstream inputFile;
    inputFile.open(path, std::ios::binary);
    tree.load(inputFile);
  }
};

int main() {
  Dictionary test;
  std::string input;
  while (std::cin >> input) {
    if (input == "") {
    }
  }
}
