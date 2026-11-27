#include "LinearProbingHashTable.hpp"

using namespace std;

LinearProbingHashTable::LinearProbingHashTable(size_t capacity)
    : cells_(capacity < 2 ? 2 : capacity) {}

size_t LinearProbingHashTable::positiveModulo(int key, size_t size) {
    const long long value = key;
    const long long result = value % static_cast<long long>(size);
    return static_cast<size_t>(result < 0 ? result + static_cast<long long>(size) : result);
}

bool LinearProbingHashTable::insert(int key, const string& value) {
    const size_t cap = cells_.size();
    const size_t start = positiveModulo(key, cap);
    optional<size_t> firstDeleted;

    for (size_t attempt = 0; attempt < cap; ++attempt) {
        const size_t index = (start + attempt) % cap;
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

optional<string> LinearProbingHashTable::find(int key) const {
    const size_t cap = cells_.size();
    const size_t start = positiveModulo(key, cap);

    for (size_t attempt = 0; attempt < cap; ++attempt) {
        const size_t index = (start + attempt) % cap;
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

bool LinearProbingHashTable::remove(int key) {
    const size_t cap = cells_.size();
    const size_t start = positiveModulo(key, cap);

    for (size_t attempt = 0; attempt < cap; ++attempt) {
        const size_t index = (start + attempt) % cap;
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

size_t LinearProbingHashTable::size() const {
    return size_;
}

size_t LinearProbingHashTable::capacity() const {
    return cells_.size();
}

size_t LinearProbingHashTable::probes() const {
    return probes_;
}

void LinearProbingHashTable::resetProbes() {
    probes_ = 0;
}
