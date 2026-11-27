#include "BinarySearchTree.hpp"

#include <algorithm>
#include <cstdlib>
#include <utility>

using namespace std;

BinarySearchTree::BinarySearchTree(const BinarySearchTree& other)
    : root_(clone(other.root_)) {}

BinarySearchTree::BinarySearchTree(BinarySearchTree&& other) noexcept
    : root_(other.root_) {
    other.root_ = nullptr;
}

BinarySearchTree& BinarySearchTree::operator=(const BinarySearchTree& other) {
    if (this == &other) {
        return *this;
    }
    BinarySearchTree copy(other);
    swap(root_, copy.root_);
    return *this;
}

BinarySearchTree& BinarySearchTree::operator=(BinarySearchTree&& other) noexcept {
    if (this == &other) {
        return *this;
    }
    destroy(root_);
    root_ = other.root_;
    other.root_ = nullptr;
    return *this;
}

BinarySearchTree::~BinarySearchTree() {
    destroy(root_);
}

BinarySearchTree::Node* BinarySearchTree::clone(const Node* node) {
    if (node == nullptr) {
        return nullptr;
    }
    Node* copy = new Node(node->value);
    copy->left = clone(node->left);
    copy->right = clone(node->right);
    return copy;
}

void BinarySearchTree::insert(int value) {
    insert(root_, value);
}

bool BinarySearchTree::isAvlBalanced() const {
    return heightOrError(root_) >= 0;
}

void BinarySearchTree::insert(Node*& node, int value) {
    if (node == nullptr) {
        node = new Node(value);
    } else if (value < node->value) {
        insert(node->left, value);
    } else if (value > node->value) {
        insert(node->right, value);
    }
}

int BinarySearchTree::heightOrError(const Node* node) {
    if (node == nullptr) {
        return 0;
    }
    const int left = heightOrError(node->left);
    const int right = heightOrError(node->right);
    if (left < 0 || right < 0 || abs(left - right) > 1) {
        return -1;
    }
    return max(left, right) + 1;
}

void BinarySearchTree::destroy(Node* node) {
    if (node == nullptr) {
        return;
    }
    destroy(node->left);
    destroy(node->right);
    delete node;
}
