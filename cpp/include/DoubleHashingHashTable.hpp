#pragma once

#include <cstddef>
#include <optional>
#include <string>
#include <vector>

using namespace std;

class DoubleHashingHashTable {
public:
    explicit DoubleHashingHashTable(size_t capacity = 29);

    bool insert(int key, const string& value);
    optional<string> find(int key) const;
    bool remove(int key);

    size_t size() const;
    size_t capacity() const;
    size_t probes() const;
    void resetProbes();

private:
    enum class SlotState { Empty, Occupied, Deleted };

    struct Entry {
        SlotState state = SlotState::Empty;
        int key = 0;
        string value;
    };

    static size_t positiveModulo(long long key, size_t size);
    static size_t nextPrime(size_t minCapacity);
    size_t secondHash(int key) const;

    vector<Entry> cells_;
    size_t size_ = 0;
    mutable size_t probes_ = 0;
};
