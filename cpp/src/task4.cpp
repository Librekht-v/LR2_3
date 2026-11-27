#include "ArrayStructure.hpp"
#include "tasks.h"

#include <iostream>
#include <string>

using namespace std;

int task4() {
    string text;
    string pattern;
    getline(cin, text);
    getline(cin, pattern);
    if (pattern.empty() && !cin) {
        cerr << "Ошибка: введите последовательность и шаблон в двух строках.\n";
        return 1;
    }

    ArrayStructure previous(pattern.size() + 1, "0");
    ArrayStructure current(pattern.size() + 1, "0");
    previous.set(0, "1");
    for (size_t column = 1; column <= pattern.size(); ++column) {
        const bool ok = previous.get(column - 1) == "1" && pattern[column - 1] == '*';
        previous.set(column, ok ? "1" : "0");
    }

    for (size_t row = 1; row <= text.size(); ++row) {
        current.fill("0");
        for (size_t column = 1; column <= pattern.size(); ++column) {
            const char mask = pattern[column - 1];
            bool match = false;
            if (mask == '*') {
                match = current.get(column - 1) == "1" || previous.get(column) == "1";
            } else if (mask == '?' || mask == text[row - 1]) {
                match = previous.get(column - 1) == "1";
            }
            current.set(column, match ? "1" : "0");
        }
        previous = current;
    }

    cout << (previous.get(pattern.size()) == "1" ? "Соответствует" : "Не соответствует") << '\n';
    return 0;
}
