#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>

using namespace std;

class SinglyLinkedList {
public:
    SinglyLinkedList() = default;
    SinglyLinkedList(const SinglyLinkedList& other);
    SinglyLinkedList(SinglyLinkedList&& other) noexcept = default;
    SinglyLinkedList& operator=(const SinglyLinkedList& other);
    SinglyLinkedList& operator=(SinglyLinkedList&& other) noexcept = default;
    ~SinglyLinkedList() = default;

    void pushHead(const string& value);
    void pushTail(const string& value);
    string removeHead();
    bool removeValue(const string& value);
    int find(const string& value) const;
    bool empty() const;
    const string& headValue() const;
    vector<string> values() const;

private:
    struct Node {
        explicit Node(string nodeValue) : value(std::move(nodeValue)) {}

        string value;
        unique_ptr<Node> next;
    };

    unique_ptr<Node> head_;
};
