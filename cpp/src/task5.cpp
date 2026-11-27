#include "BinarySearchTree.hpp"
#include "tasks.h"

#include <iostream>

using namespace std;

int task5() {
    BinarySearchTree tree;
    int value = 0;
    bool hasValues = false;
    while (cin >> value && value != 0) {
        tree.insert(value);
        hasValues = true;
    }
    if (!hasValues) {
        cerr << "Ошибка: дерево не содержит элементов.\n";
        return 1;
    }
    cout << (tree.isAvlBalanced() ? "YES" : "NO") << '\n';
    return 0;
}
