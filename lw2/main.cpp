#include <iostream>
#include <string>

using T = int;

enum colorType { red, black };

struct Node {
  colorType color;
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

  Node(colorType colorValue, const T& keyValue, Node* leftChild,
       Node* rightChild, Node* parentNode)
      : color(colorValue),
        key(keyValue),
        left(leftChild),
        right(rightChild),
        parent(parentNode) {}

  bool isLeftSon() const { return this == this->parent->left; }

  bool isRightSon() const { return !isLeftSon(); }
};

class RedBlackTree {
 private:
  Node* nullNode;
  Node* root;

  void deleteSubtree(Node* root) {
    if (root == nullptr || root == nullNode) {
      return;
    }
    if (root->left != nullNode) {
      deleteSubtree(root->left);
    }
    if (root->right != nullNode) {
      deleteSubtree(root->right);
    }
    if (root->parent == nullNode) {
      this->root = nullNode;
    } else if (root->isLeftSon()) {
      root->parent->left = nullNode;
    } else {
      root->parent->right = nullNode;
    }
    delete root;
  }

  void leftRotate(Node* oldRoot) {
    if (oldRoot->right == nullNode) {
      throw std::runtime_error("Unable to do left rotate");
    }
    Node* newRoot = oldRoot->right;
    oldRoot->right = newRoot->left;
    if (newRoot->left != nullNode) {
      newRoot->left->parent = oldRoot;
    }
    newRoot->parent = oldRoot->parent;
    if (oldRoot->parent == nullNode) {
      this->root = newRoot;
    } else if (oldRoot->isLeftSon()) {
      oldRoot->parent->left = newRoot;
    } else {
      oldRoot->parent->right = newRoot;
    }
    newRoot->left = oldRoot;
    oldRoot->parent = newRoot;
  }

  void rightRotate(Node* oldRoot) {
    if (oldRoot->left == nullNode) {
      throw std::runtime_error("Unable to do right rotate");
    }
    Node* newRoot = oldRoot->left;
    oldRoot->left = newRoot->right;
    if (newRoot->right != nullNode) {
      newRoot->right->parent = oldRoot;
    }
    newRoot->parent = oldRoot->parent;
    if (oldRoot->parent == nullNode) {
      this->root = newRoot;
    } else if (oldRoot->isLeftSon()) {
      oldRoot->parent->left = newRoot;
    } else {
      oldRoot->parent->right = newRoot;
    }
    newRoot->right = oldRoot;
    oldRoot->parent = newRoot;
  }

  void insertFixup(Node* brokenNode) {
    while (brokenNode->parent->color != black) {
      Node* uncle = brokenNode->parent->isLeftSon()
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

  void replace(Node* oldSubtree, Node* newSubtree) {
    if (oldSubtree->parent == nullNode) {
      root = newSubtree;
    } else if (oldSubtree->isLeftSon()) {
      oldSubtree->parent->left = newSubtree;
    } else {
      oldSubtree->parent->right = newSubtree;
    }
    if (newSubtree != nullNode) {
      newSubtree->parent = oldSubtree->parent;
    }
  }

  void eraseFixup(Node* brokenNode) {
    while (brokenNode != root && brokenNode->color != black) {
      Node* brother = brokenNode->isLeftSon() ? brokenNode->parent->right
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

 public:
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

  Node* min(Node* root = nullptr) {
    if (root == nullptr) {
      root = this->root;
    }
    if (root == nullNode) {
      return root;
    }
    while (root->left != nullNode) {
      root = root->left;
    }
    return root;
  }

  Node* max(Node* root = nullptr) {
    if (root == nullptr) {
      root = this->root;
    }
    if (root == nullNode) {
      return root;
    }
    while (root->right != nullNode) {
      root = root->right;
    }
    return root;
  }

  void erase(Node* target) {
    colorType erasedColor = target->color;
    Node* movedNode;
    if (target->left == nullNode) {
      movedNode = target->right;
      replace(target, target->right);
    } else if (target->right == nullNode) {
      movedNode = target->left;
      replace(target, target->left);
    } else {
      Node* rightSubtreeMinimum = min(target->right);
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
  }

  void clear() { deleteSubtree(root); }
};

int main() {
  RedBlackTree tree{};
  tree.insert(3);
  tree.insert(14);
  tree.insert(15);
  tree.insert(9);
  tree.insert(2);
  tree.insert(6);
  tree.insert(5);

  tree.erase(tree.find(15));
  tree.erase(tree.find(3));
  tree.erase(tree.find(9));
}