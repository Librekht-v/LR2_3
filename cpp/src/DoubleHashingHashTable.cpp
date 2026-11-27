#include "DoubleHashingHashTable.hpp"

using namespace std;

DoubleHashingHashTable::DoubleHashingHashTable(size_t capacity)
    : cells_(nextPrime(capacity < 3 ? 3 : capacity)) {}

size_t DoubleHashingHashTable::positiveModulo(long long key, size_t size) {
    const long long result = key % static_cast<long long>(size);
    return static_cast<size_t>(result < 0 ? result + static_cast<long long>(size) : result);
}

size_t DoubleHashingHashTable::nextPrime(size_t minCapacity) {
    auto isPrime = [](size_t n) {
        if (n < 2) return false;
        for (size_t d = 2; d * d <= n; ++d) {
            if (n % d == 0) return false;
        }
        return true;
    };
    size_t candidate = minCapacity;
    while (!isPrime(candidate)) {
        ++candidate;
    }
    return candidate;
}

size_t DoubleHashingHashTable::secondHash(int key) const {
    return 1 + positiveModulo(static_cast<long long>(key) * 37LL + 17LL, cells_.size() - 1);
}

bool DoubleHashingHashTable::insert(int key, const string& value) {
    const size_t cap = cells_.size();
    const size_t start = positiveModulo(key, cap);
    const size_t step = secondHash(key);
    optional<size_t> firstDeleted;

    for (size_t attempt = 0; attempt < cap; ++attempt) {
        const size_t index = (start + attempt * step) % cap;
        ++probes_;
        Entry& cell = cells_[index];
        if (cell.state == SlotState::Empty) {
            const size_t target = firstDeleted.value_or(index);
            cells_[target].state = SlotState::Occupied;
            cells_[target].key = key;
            cells_[target].value = value;
            ++size_;
            return true;
        }
        if (cell.state == SlotState::Deleted) {
            if (!firstDeleted.has_value()) {
                firstDeleted = index;
            }
        } else if (cell.key == key) {
            cell.value = value;
            return false;
        }
    }

    if (firstDeleted.has_value()) {
        const size_t target = *firstDeleted;
        cells_[target].state = SlotState::Occupied;
        cells_[target].key = key;
        cells_[target].value = value;
        ++size_;
        return true;
    }
    return false;
}

optional<string> DoubleHashingHashTable::find(int key) const {
    const size_t cap = cells_.size();
    const size_t start = positiveModulo(key, cap);
    const size_t step = secondHash(key);

    for (size_t attempt = 0; attempt < cap; ++attempt) {
        const size_t index = (start + attempt * step) % cap;
        ++probes_;
        const Entry& cell = cells_[index];
        if (cell.state == SlotState::Empty) {
            return nullopt;
        }
        if (cell.state == SlotState::Occupied && cell.key == key) {
            return cell.value;
        }
    }
    return nullopt;
}

bool DoubleHashingHashTable::remove(int key) {
    const size_t cap = cells_.size();
    const size_t start = positiveModulo(key, cap);
    const size_t step = secondHash(key);

    for (size_t attempt = 0; attempt < cap; ++attempt) {
        const size_t index = (start + attempt * step) % cap;
        ++probes_;
        Entry& cell = cells_[index];
        if (cell.state == SlotState::Empty) {
            return false;
        }
        if (cell.state == SlotState::Occupied && cell.key == key) {
            cell.state = SlotState::Deleted;
            cell.value.clear();
            --size_;
            return true;
        }
    }
    return false;
}

size_t DoubleHashingHashTable::size() const {
    return size_;
}

size_t DoubleHashingHashTable::capacity() const {
    return cells_.size();
}

size_t DoubleHashingHashTable::probes() const {
    return probes_;
}

void DoubleHashingHashTable::resetProbes() {
    probes_ = 0;
}
