#include "ArrayStructure.hpp"

#include <stdexcept>
#include <utility>

using namespace std;

ArrayStructure::ArrayStructure(size_t initialSize, const string& initialValue) {
    if (initialSize == 0) {
        return;
    }
    reserve(initialSize);
    for (size_t i = 0; i < initialSize; ++i) {
        data_[i] = initialValue;
    }
    size_ = initialSize;
}

ArrayStructure::ArrayStructure(const ArrayStructure& other) {
    reserve(other.size_);
    for (size_t i = 0; i < other.size_; ++i) {
        data_[i] = other.data_[i];
    }
    size_ = other.size_;
}

ArrayStructure::ArrayStructure(ArrayStructure&& other) noexcept
    : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
}

ArrayStructure& ArrayStructure::operator=(const ArrayStructure& other) {
    if (this == &other) {
        return *this;
    }
    ArrayStructure copy(other);
    swap(data_, copy.data_);
    swap(size_, copy.size_);
    swap(capacity_, copy.capacity_);
    return *this;
}

ArrayStructure& ArrayStructure::operator=(ArrayStructure&& other) noexcept {
    if (this == &other) {
        return *this;
    }
    delete[] data_;
    data_ = other.data_;
    size_ = other.size_;
    capacity_ = other.capacity_;
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
    return *this;
}

ArrayStructure::~ArrayStructure() {
    delete[] data_;
}

void ArrayStructure::reserve(size_t requiredCapacity) {
    if (requiredCapacity <= capacity_) {
        return;
    }
    size_t newCapacity = capacity_ == 0 ? 4 : capacity_;
    while (newCapacity < requiredCapacity) {
        newCapacity *= 2;
    }
    string* newData = new string[newCapacity];
    for (size_t i = 0; i < size_; ++i) {
        newData[i] = std::move(data_[i]);
    }
    delete[] data_;
    data_ = newData;
    capacity_ = newCapacity;
}

void ArrayStructure::pushBack(const string& value) {
    reserve(size_ + 1);
    data_[size_++] = value;
}

void ArrayStructure::insert(size_t index, const string& value) {
    if (index > size_) {
        throw out_of_range("индекс массива вне диапазона");
    }
    reserve(size_ + 1);
    for (size_t i = size_; i > index; --i) {
        data_[i] = std::move(data_[i - 1]);
    }
    data_[index] = value;
    ++size_;
}

string ArrayStructure::get(size_t index) const {
    if (index >= size_) {
        throw out_of_range("индекс массива вне диапазона");
    }
    return data_[index];
}

string ArrayStructure::removeAt(size_t index) {
    if (index >= size_) {
        throw out_of_range("индекс массива вне диапазона");
    }
    string removed = data_[index];
    for (size_t i = index; i + 1 < size_; ++i) {
        data_[i] = std::move(data_[i + 1]);
    }
    --size_;
    data_[size_].clear();
    return removed;
}

void ArrayStructure::set(size_t index, const string& value) {
    if (index >= size_) {
        throw out_of_range("индекс массива вне диапазона");
    }
    data_[index] = value;
}

void ArrayStructure::fill(const string& value) {
    for (size_t i = 0; i < size_; ++i) {
        data_[i] = value;
    }
}

size_t ArrayStructure::length() const {
    return size_;
}
