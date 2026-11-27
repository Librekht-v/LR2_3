#include "SinglyLinkedList.hpp"

#include <stdexcept>
#include <utility>

using namespace std;

SinglyLinkedList::SinglyLinkedList(const SinglyLinkedList& other) {
    for (const string& value : other.values()) {
        pushTail(value);
    }
}

SinglyLinkedList& SinglyLinkedList::operator=(const SinglyLinkedList& other) {
    if (this == &other) {
        return *this;
    }
    SinglyLinkedList copy(other);
    head_ = std::move(copy.head_);
    return *this;
}

void SinglyLinkedList::pushHead(const string& value) {
    auto node = make_unique<Node>(value);
    node->next = std::move(head_);
    head_ = std::move(node);
}

void SinglyLinkedList::pushTail(const string& value) {
    auto node = make_unique<Node>(value);
    if (!head_) {
        head_ = std::move(node);
        return;
    }
    Node* current = head_.get();
    while (current->next) {
        current = current->next.get();
    }
    current->next = std::move(node);
}

string SinglyLinkedList::removeHead() {
    if (!head_) {
        throw runtime_error("список пуст");
    }
    string removed = std::move(head_->value);
    head_ = std::move(head_->next);
    return removed;
}

bool SinglyLinkedList::removeValue(const string& value) {
    if (!head_) {
        return false;
    }
    if (head_->value == value) {
        removeHead();
        return true;
    }
    Node* previous = head_.get();
    while (previous->next && previous->next->value != value) {
        previous = previous->next.get();
    }
    if (!previous->next) {
        return false;
    }
    previous->next = std::move(previous->next->next);
    return true;
}

int SinglyLinkedList::find(const string& value) const {
    const Node* current = head_.get();
    int index = 0;
    while (current) {
        if (current->value == value) {
            return index;
        }
        current = current->next.get();
        ++index;
    }
    return -1;
}

bool SinglyLinkedList::empty() const {
    return head_ == nullptr;
}

const string& SinglyLinkedList::headValue() const {
    if (!head_) {
        throw runtime_error("список пуст");
    }
    return head_->value;
}

vector<string> SinglyLinkedList::values() const {
    vector<string> result;
    const Node* current = head_.get();
    while (current) {
        result.push_back(current->value);
        current = current->next.get();
    }
    return result;
}
