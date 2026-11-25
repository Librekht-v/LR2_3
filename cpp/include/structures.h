#pragma once

#include <algorithm>
#include <cstddef>
#include <functional>
#include <list>
#include <optional>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

using namespace std;

template <typename T>
class Stack {
public:
    void push(const T& value) { data_.push_back(value); }
    void pop() {
        if (data_.empty()) throw runtime_error("stack is empty");
        data_.pop_back();
    }
    const T& top() const {
        if (data_.empty()) throw runtime_error("stack is empty");
        return data_.back();
    }
    bool empty() const { return data_.empty(); }
    size_t size() const { return data_.size(); }

private:
    vector<T> data_;
};

template <typename T>
class DynamicArray {
public:
    explicit DynamicArray(size_t size = 0, const T& value = T()) : data_(size, value) {}
    T& operator[](size_t index) { return data_.at(index); }
    const T& operator[](size_t index) const { return data_.at(index); }
    size_t size() const { return data_.size(); }
    void resize(size_t size, const T& value = T()) { data_.resize(size, value); }
    void fill(const T& value) { std::fill(data_.begin(), data_.end(), value); }

private:
    vector<T> data_;
};

class StringSet {
public:
    explicit StringSet(size_t buckets = 101) : buckets_(buckets) {}

    bool add(const string& value) {
        auto& bucket = buckets_[index(value)];
        if (find(bucket.begin(), bucket.end(), value) != bucket.end()) return false;
        bucket.push_back(value);
        ++size_;
        return true;
    }

    bool erase(const string& value) {
        auto& bucket = buckets_[index(value)];
        auto position = find(bucket.begin(), bucket.end(), value);
        if (position == bucket.end()) return false;
        bucket.erase(position);
        --size_;
        return true;
    }

    bool contains(const string& value) const {
        const auto& bucket = buckets_[index(value)];
        return find(bucket.begin(), bucket.end(), value) != bucket.end();
    }

    vector<string> values() const {
        vector<string> result;
        result.reserve(size_);
        for (const auto& bucket : buckets_)
            result.insert(result.end(), bucket.begin(), bucket.end());
        return result;
    }

    size_t size() const { return size_; }

private:
    size_t index(const string& value) const { return hash<string>{}(value) % buckets_.size(); }
    vector<list<string>> buckets_;
    size_t size_ = 0;
};

class BinarySearchTree {
public:
    ~BinarySearchTree() { destroy(root_); }

    void insert(int value) { insert(root_, value); }

    bool isAvlBalanced() const { return heightOrError(root_) >= 0; }

private:
    struct Node {
        explicit Node(int value) : value(value) {}
        int value;
        Node* left = nullptr;
        Node* right = nullptr;
    };

    static void insert(Node*& node, int value) {
        if (node == nullptr) {
            node = new Node(value);
        } else if (value < node->value) {
            insert(node->left, value);
        } else if (value > node->value) {
            insert(node->right, value);
        }
    }

    static int heightOrError(const Node* node) {
        if (node == nullptr) return 0;
        const int left = heightOrError(node->left);
        const int right = heightOrError(node->right);
        if (left < 0 || right < 0 || abs(left - right) > 1) return -1;
        return max(left, right) + 1;
    }

    static void destroy(Node* node) {
        if (node == nullptr) return;
        destroy(node->left);
        destroy(node->right);
        delete node;
    }

    Node* root_ = nullptr;
};

class ChainedIntSet {
public:
    explicit ChainedIntSet(size_t buckets = 17) : buckets_(buckets) {}

    bool insert(int key) {
        auto& bucket = buckets_[index(key)];
        if (find(bucket.begin(), bucket.end(), key) != bucket.end()) return false;
        bucket.push_back(key);
        ++size_;
        return true;
    }

    bool contains(int key) const {
        const auto& bucket = buckets_[index(key)];
        return find(bucket.begin(), bucket.end(), key) != bucket.end();
    }

    size_t size() const { return size_; }

private:
    size_t index(int key) const { return hash<int>{}(key) % buckets_.size(); }
    vector<list<int>> buckets_;
    size_t size_ = 0;
};
