#pragma once

#include <cstddef>
#include <string>

using namespace std;

class ArrayStructure {
public:
    explicit ArrayStructure(size_t initialSize = 0, const string& initialValue = "");
    ArrayStructure(const ArrayStructure& other);
    ArrayStructure(ArrayStructure&& other) noexcept;
    ArrayStructure& operator=(const ArrayStructure& other);
    ArrayStructure& operator=(ArrayStructure&& other) noexcept;
    ~ArrayStructure();

    void pushBack(const string& value);
    void insert(size_t index, const string& value);
    string get(size_t index) const;
    string removeAt(size_t index);
    void set(size_t index, const string& value);
    void fill(const string& value);
    size_t length() const;

private:
    void reserve(size_t requiredCapacity);

    string* data_ = nullptr;
    size_t size_ = 0;
    size_t capacity_ = 0;
};
