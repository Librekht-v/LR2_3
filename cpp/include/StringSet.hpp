#pragma once

#include "SinglyLinkedList.hpp"

#include <cstddef>
#include <string>
#include <vector>

using namespace std;

class StringSet {
public:
    explicit StringSet(size_t buckets = 101);

    bool add(const string& value);
    bool erase(const string& value);
    bool contains(const string& value) const;
    vector<string> values() const;
    size_t size() const;

private:
    size_t index(const string& value) const;

    vector<SinglyLinkedList> buckets_;
    size_t size_ = 0;
};
