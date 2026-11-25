#include "tasks.h"

#include <iostream>
#include <list>
#include <string>
#include <unordered_map>

using namespace std;

namespace {
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

class LfuCache {
public:
    explicit LfuCache(size_t capacity) : capacity_(capacity) {}

    void set(int key, int value) {
        auto found = values_.find(key);
        if (found != values_.end()) {
            found->second.value = value;
            touch(found);
            return;
        }
        if (values_.size() == capacity_) evict();
        values_[key] = {value, 1, ++clock_};
    }

    int get(int key) {
        auto found = values_.find(key);
        if (found == values_.end()) return -1;
        touch(found);
        return found->second.value;
    }

private:
    struct Entry {
        int value;
        size_t frequency;
        size_t lastUse;
    };

    void touch(unordered_map<int, Entry>::iterator found) {
        ++found->second.frequency;
        found->second.lastUse = ++clock_;
    }

    void evict() {
        auto victim = values_.begin();
        for (auto current = values_.begin(); current != values_.end(); ++current) {
            if (current->second.frequency < victim->second.frequency ||
                (current->second.frequency == victim->second.frequency &&
                 current->second.lastUse < victim->second.lastUse)) {
                victim = current;
            }
        }
        values_.erase(victim);
    }

    size_t capacity_;
    size_t clock_ = 0;
    unordered_map<int, Entry> values_;
};
}

int task7() {
    string type;
    size_t capacity = 0;
    size_t queryCount = 0;
    if (!(cin >> type >> capacity >> queryCount) || capacity == 0) {
        cerr << "Формат: LRU или LFU, затем ёмкость и число запросов.\n";
        return 1;
    }
    if (type != "LRU" && type != "LFU") {
        cerr << "Ошибка: тип кэша должен быть LRU или LFU.\n";
        return 1;
    }

    LruCache lru(capacity);
    LfuCache lfu(capacity);
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
            if (type == "LRU") lru.set(key, value);
            else lfu.set(key, value);
        } else if (operation == "GET") {
            cout << (type == "LRU" ? lru.get(key) : lfu.get(key)) << '\n';
        } else {
            cerr << "Ошибка: неизвестная операция.\n";
            return 1;
        }
    }
    return 0;
}
