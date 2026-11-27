#pragma once

#include <cstddef>
#include <unordered_map>

using namespace std;

class LruCache {
public:
    explicit LruCache(size_t capacity);
    LruCache(const LruCache&) = delete;
    LruCache& operator=(const LruCache&) = delete;
    ~LruCache();

    void set(int key, int value);
    int get(int key);

private:
    struct Node {
        Node(int k, int v) : key(k), value(v) {}

        int key;
        int value;
        Node* prev = nullptr;
        Node* next = nullptr;
    };

    void moveToFront(Node* node);
    void pushFront(Node* node);
    void removeBack();

    size_t capacity_;
    Node* head_ = nullptr;
    Node* tail_ = nullptr;
    unordered_map<int, Node*> nodes_;
};
