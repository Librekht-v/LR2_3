#pragma once

#include "SinglyLinkedList.hpp"

#include <string>

using namespace std;

class StackStructure {
public:
    void push(const string& value);
    string pop();
    const string& top() const;
    bool empty() const;

private:
    SinglyLinkedList list_;
};
