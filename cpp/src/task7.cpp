#include "tasks.h"

#include <iostream>
#include <list>
#include <string>
#include <unordered_map>

using namespace std;

class LruCache {
public:
    explicit LruCache(size_t capacity) : capacity_(capacity) {}

    void set(int key, int value) {
        auto found = values_.find(key);
        if (found != values_.end()) {
            found->second.first = value;
            touch(found);
            return;
        }
        if (values_.size() == capacity_) {
            values_.erase(order_.back());
            order_.pop_back();
        }
        order_.push_front(key);
        values_[key] = {value, order_.begin()};
    }

    int get(int key) {
        auto found = values_.find(key);
        if (found == values_.end()) return -1;
        touch(found);
        return found->second.first;
    }

private:
    using Entry = pair<int, list<int>::iterator>;
    void touch(unordered_map<int, Entry>::iterator found) {
        order_.erase(found->second.second);
        order_.push_front(found->first);
        found->second.second = order_.begin();
    }

    size_t capacity_;
    list<int> order_;
    unordered_map<int, Entry> values_;
};

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
