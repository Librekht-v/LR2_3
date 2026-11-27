#include "StringSet.hpp"

#include <functional>

using namespace std;

StringSet::StringSet(size_t buckets) : buckets_(buckets == 0 ? 1 : buckets) {}

size_t StringSet::index(const string& value) const {
    return hash<string>{}(value) % buckets_.size();
}

bool StringSet::add(const string& value) {
    SinglyLinkedList& bucket = buckets_[index(value)];
    if (bucket.find(value) != -1) {
        return false;
    }
    bucket.pushTail(value);
    ++size_;
    return true;
}

bool StringSet::erase(const string& value) {
    SinglyLinkedList& bucket = buckets_[index(value)];
    if (!bucket.removeValue(value)) {
        return false;
    }
    --size_;
    return true;
}

bool StringSet::contains(const string& value) const {
    const SinglyLinkedList& bucket = buckets_[index(value)];
    return bucket.find(value) != -1;
}

vector<string> StringSet::values() const {
    vector<string> result;
    result.reserve(size_);
    for (const SinglyLinkedList& bucket : buckets_) {
        vector<string> items = bucket.values();
        result.insert(result.end(), items.begin(), items.end());
    }
    return result;
}

size_t StringSet::size() const {
    return size_;
}
