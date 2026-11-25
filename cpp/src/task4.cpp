#include "structures.h"
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

    DynamicArray<unsigned char> previous(pattern.size() + 1, 0);
    DynamicArray<unsigned char> current(pattern.size() + 1, 0);
    previous[0] = true;
    for (size_t column = 1; column <= pattern.size(); ++column)
        previous[column] = previous[column - 1] && pattern[column - 1] == '*';

    for (size_t row = 1; row <= text.size(); ++row) {
        current.fill(false);
        for (size_t column = 1; column <= pattern.size(); ++column) {
            const char mask = pattern[column - 1];
            if (mask == '*') {
                current[column] = current[column - 1] || previous[column];
            } else if (mask == '?' || mask == text[row - 1]) {
                current[column] = previous[column - 1];
            }
        }
        previous = current;
    }

    cout << (previous[pattern.size()] ? "Соответствует" : "Не соответствует") << '\n';
    return 0;
}
