#pragma once

class BinarySearchTree {
public:
    BinarySearchTree() = default;
    BinarySearchTree(const BinarySearchTree& other);
    BinarySearchTree(BinarySearchTree&& other) noexcept;
    BinarySearchTree& operator=(const BinarySearchTree& other);
    BinarySearchTree& operator=(BinarySearchTree&& other) noexcept;
    ~BinarySearchTree();

    void insert(int value);
    bool isAvlBalanced() const;

private:
    struct Node {
        explicit Node(int nodeValue) : value(nodeValue) {}

        int value;
        Node* left = nullptr;
        Node* right = nullptr;
    };

    static Node* clone(const Node* node);
    static void insert(Node*& node, int value);
    static int heightOrError(const Node* node);
    static void destroy(Node* node);

    Node* root_ = nullptr;
};
