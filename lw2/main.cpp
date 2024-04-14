#include <iostream>
#include <string>

using T = int;

enum nodeColor { red, black };

struct Node {
  char color;
  T key;
  Node* left;
  Node* right;
  Node* parent;

  Node()
      : color(black),
        key(T()),
        left(nullptr),
        right(nullptr),
        parent(nullptr) {}

  Node(char colorValue, const T& keyValue, Node* leftChild, Node* rightChild,
       Node* parentNode)
      : color(colorValue),
        key(keyValue),
        left(leftChild),
        right(rightChild),
        parent(parentNode) {}
};

class RedBlackTree {
 private:
  Node* nullNode;
  Node* root;

 public:
  void deleteSubtree(Node*& root) {
    if (root->left != nullNode) {
      deleteSubtree(root->left);
    }
    if (root->right != nullNode) {
      deleteSubtree(root->right);
    }
    if (root->parent != nullNode) {
      if (isLeftSon(root))
        root->parent->left = nullNode;
      else
        root->parent->right = nullNode;
    }
    delete root;
    root = nullNode;
  }

  RedBlackTree() {
    nullNode = new Node();
    root = nullNode;
    root->parent = nullNode;
  }

  ~RedBlackTree() {
    clear();
    delete nullNode;
    nullNode = root = nullptr;
  }

  Node* find(const T& key) const noexcept {
    Node* foundNode = root;
    while (foundNode != nullNode && foundNode->key != key) {
      foundNode = key < foundNode->key ? foundNode->left : foundNode->right;
    }
    return foundNode;
  }

  // bool isRoot(const Node* currentNode) const {
  //   return currentNode->parent == nullNode;
  // }

  bool isLeftSon(const Node* child) const {
    return child == child->parent->left;
  }

  bool isRightSon(const Node* child) const { return !isLeftSon(child); }

  void leftRotate(Node* currentNode) {
    if (currentNode->right == nullNode) {
      throw std::runtime_error("Unable to do left rotate");
    }
    Node* newParent = currentNode->right;
    currentNode->right = newParent->left;
    if (newParent->left != nullNode) {
      newParent->left->parent = currentNode;
    }
    newParent->parent = currentNode->parent;
    if (currentNode->parent == nullNode) {
      this->root = newParent;
    } else if (isLeftSon(currentNode)) {
      currentNode->parent->left = newParent;
    } else {
      currentNode->parent->right = newParent;
    }
    newParent->left = currentNode;
    currentNode->parent = newParent;
  }

  void rightRotate(Node* currentNode) {
    if (currentNode->left == nullNode) {
      throw std::runtime_error("Unable to do right rotate");
    }
    Node* newParent = currentNode->left;
    currentNode->left = newParent->right;
    if (newParent->right != nullNode) {
      newParent->right->parent = currentNode;
    }
    newParent->parent = currentNode->parent;
    if (currentNode->parent == nullNode) {
      this->root = newParent;
    } else if (isLeftSon(currentNode)) {
      currentNode->parent->left = newParent;
    } else {
      currentNode->parent->right = newParent;
    }
    newParent->right = currentNode;
    currentNode->parent = newParent;
  }

  void insertFixup(Node* currentNode) {
    if (currentNode->parent->color == black) {
      root->color = black;
      return;
    }
    Node* uncle = isLeftSon(currentNode->parent)
                      ? currentNode->parent->parent->right
                      : currentNode->parent->parent->left;
    if (uncle->color == red) {
      currentNode->parent->color = black;
      uncle->color = black;
      currentNode->parent->parent->color = red;
      insertFixup(currentNode->parent->parent);
    } else {
      if (isLeftSon(currentNode->parent) && isRightSon(currentNode)) {
        currentNode = currentNode->parent;
        leftRotate(currentNode);
      } else if (isRightSon(currentNode->parent) && isLeftSon(currentNode)) {
        currentNode = currentNode->parent;
        rightRotate(currentNode);
      }

      currentNode->parent->color = black;
      currentNode->parent->parent->color = red;
      if (isLeftSon(currentNode->parent)) {
        rightRotate(currentNode->parent->parent);
      } else {
        leftRotate(currentNode->parent->parent);
      }
    }
    root->color = black;
  }

  void insert(const T& key) {
    if (find(key) != nullNode) {
    }
    Node* parent = nullNode;
    Node* child = root;
    while (child != nullNode) {
      parent = child;
      if (key < child->key) {
        child = child->left;
      } else if (key > child->key) {
        child = child->right;
      } else {
        throw std::runtime_error("Node already exist");
      }
    }
    Node* newChild = new Node(red, key, nullNode, nullNode, parent);
    if (parent == nullNode) {
      root = newChild;
    } else if (key < parent->key) {
      parent->left = newChild;
    } else {
      parent->right = newChild;
    }
    insertFixup(newChild);
  }

  void transplant(Node* currentSubtree, Node* newSubtree) {
    if (currentSubtree->parent == nullNode) {
      root = newSubtree;
    } else if (isLeftSon(currentSubtree)) {
      currentSubtree->parent->left = newSubtree;
    } else {
      currentSubtree->parent->right = newSubtree;
    }
    newSubtree->parent = currentSubtree->parent;
  }

  void erase(Node* currentNode) {}

  void clear() { deleteSubtree(root); }
};
