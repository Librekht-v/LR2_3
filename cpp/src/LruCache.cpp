#include "LruCache.hpp"

using namespace std;

LruCache::LruCache(size_t capacity) : capacity_(capacity) {}

LruCache::~LruCache() {
    while (head_ != nullptr) {
        Node* next = head_->next;
        delete head_;
        head_ = next;
    }
}

void LruCache::pushFront(Node* node) {
    node->prev = nullptr;
    node->next = head_;
    if (head_ != nullptr) {
        head_->prev = node;
    } else {
        tail_ = node;
    }
    head_ = node;
}

void LruCache::moveToFront(Node* node) {
    if (node == head_) {
        return;
    }
    if (node->prev != nullptr) {
        node->prev->next = node->next;
    }
    if (node->next != nullptr) {
        node->next->prev = node->prev;
    } else {
        tail_ = node->prev;
    }
    pushFront(node);
}

void LruCache::removeBack() {
    if (tail_ == nullptr) {
        return;
    }
    Node* victim = tail_;
    nodes_.erase(victim->key);
    if (victim->prev != nullptr) {
        victim->prev->next = nullptr;
        tail_ = victim->prev;
    } else {
        head_ = nullptr;
        tail_ = nullptr;
    }
    delete victim;
}

void LruCache::set(int key, int value) {
    if (capacity_ == 0) {
        return;
    }
    auto found = nodes_.find(key);
    if (found != nodes_.end()) {
        found->second->value = value;
        moveToFront(found->second);
        return;
    }
    if (nodes_.size() == capacity_) {
        removeBack();
    }
    Node* node = new Node(key, value);
    pushFront(node);
    nodes_[key] = node;
}

int LruCache::get(int key) {
    auto found = nodes_.find(key);
    if (found == nodes_.end()) {
        return -1;
    }
    moveToFront(found->second);
    return found->second->value;
}
