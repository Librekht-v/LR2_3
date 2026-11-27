#include "LruCache.hpp"
#include "tasks.h"

#include <iostream>
#include <string>

using namespace std;

int task7() {
    string type;
    size_t capacity = 0;
    size_t queryCount = 0;
    if (!(cin >> type >> capacity >> queryCount) || capacity == 0) {
        cerr << "Формат: LRU, затем ёмкость и число запросов.\n";
        return 1;
    }
    if (type != "LRU") {
        cerr << "Ошибка: для задания 7 реализован LRU-кэш.\n";
        return 1;
    }

    LruCache lru(capacity);
    for (size_t index = 0; index < queryCount; ++index) {
        string operation;
        int key = 0;
        if (!(cin >> operation >> key)) {
            cerr << "Ошибка: неполный запрос.\n";
            return 1;
        }
        if (operation == "SET") {
            int value = 0;
            if (!(cin >> value)) {
                cerr << "Ошибка: для SET нужно значение.\n";
                return 1;
            }
            lru.set(key, value);
        } else if (operation == "GET") {
            cout << lru.get(key) << '\n';
        } else {
            cerr << "Ошибка: неизвестная операция.\n";
            return 1;
        }
    }
    return 0;
}
