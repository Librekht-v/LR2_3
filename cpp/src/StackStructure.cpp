#include "StackStructure.hpp"

#include <stdexcept>

using namespace std;

void StackStructure::push(const string& value) {
    list_.pushHead(value);
}

string StackStructure::pop() {
    if (list_.empty()) {
        throw runtime_error("стек пуст");
    }
    return list_.removeHead();
}

const string& StackStructure::top() const {
    if (list_.empty()) {
        throw runtime_error("стек пуст");
    }
    return list_.headValue();
}

bool StackStructure::empty() const {
    return list_.empty();
}
