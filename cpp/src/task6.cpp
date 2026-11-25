#include "tasks.h"

#include <chrono>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <list>
#include <optional>
#include <string>
#include <utility>
#include <vector>

using namespace std;

namespace {
size_t positiveModulo(int key, size_t size) {
    const long long value = key;
    const long long result = value % static_cast<long long>(size);
    return static_cast<size_t>(result < 0 ? result + static_cast<long long>(size) : result);
}

class LinearProbingSet {
public:
    explicit LinearProbingSet(size_t size) : cells_(size) {}

    bool insert(int key) {
        for (size_t attempt = 0; attempt < cells_.size(); ++attempt) {
            size_t index = (positiveModulo(key, cells_.size()) + attempt) % cells_.size();
            ++probes_;
            if (!cells_[index].has_value()) {
                cells_[index] = key;
                return true;
            }
            if (cells_[index] == key) return false;
        }
        return false;
    }

    size_t probes() const { return probes_; }

private:
    vector<optional<int>> cells_;
    size_t probes_ = 0;
};

class DoubleHashingSet {
public:
    explicit DoubleHashingSet(size_t size) : cells_(size) {}

    bool insert(int key) {
        const size_t first = positiveModulo(key, cells_.size());
        const size_t step = 1 + positiveModulo(key * 37 + 17, cells_.size() - 1);
        for (size_t attempt = 0; attempt < cells_.size(); ++attempt) {
            const size_t index = (first + attempt * step) % cells_.size();
            ++probes_;
            if (!cells_[index].has_value()) {
                cells_[index] = key;
                return true;
            }
            if (cells_[index] == key) return false;
        }
        return false;
    }

    size_t probes() const { return probes_; }

private:
    vector<optional<int>> cells_;
    size_t probes_ = 0;
};

class ChainingSet {
public:
    explicit ChainingSet(size_t size) : buckets_(size) {}

    bool insert(int key) {
        auto& bucket = buckets_[positiveModulo(key, buckets_.size())];
        for (int value : bucket) {
            ++comparisons_;
            if (value == key) return false;
        }
        bucket.push_back(key);
        return true;
    }

    size_t comparisons() const { return comparisons_; }

private:
    vector<list<int>> buckets_;
    size_t comparisons_ = 0;
};

class CuckooSet {
public:
    explicit CuckooSet(size_t size) : first_(size), second_(size) {}

    bool insert(int key) {
        if (contains(key)) return false;
        if (!place(key, first_, second_)) {
            vector<int> values = allValues();
            values.push_back(key);
            rebuild(values);
        }
        return true;
    }

    size_t displacements() const { return displacements_; }
    size_t rebuilds() const { return rebuilds_; }

private:
    size_t firstIndex(int key, size_t size) const { return positiveModulo(key, size); }
    size_t secondIndex(int key, size_t size) const { return positiveModulo(key * 53 + 19, size); }

    bool contains(int key) const {
        return first_[firstIndex(key, first_.size())] == key || second_[secondIndex(key, second_.size())] == key;
    }

    bool place(int key, vector<optional<int>>& first, vector<optional<int>>& second) {
        bool useFirst = true;
        int current = key;
        const size_t limit = first.size() * 2;
        for (size_t attempt = 0; attempt < limit; ++attempt) {
            vector<optional<int>>& table = useFirst ? first : second;
            const size_t index = useFirst ? firstIndex(current, table.size()) : secondIndex(current, table.size());
            if (!table[index].has_value()) {
                table[index] = current;
                return true;
            }
            swap(current, *table[index]);
            ++displacements_;
            useFirst = !useFirst;
        }
        return false;
    }

    vector<int> allValues() const {
        vector<int> values;
        for (const auto& value : first_) if (value.has_value()) values.push_back(*value);
        for (const auto& value : second_) if (value.has_value()) values.push_back(*value);
        return values;
    }

    void rebuild(const vector<int>& values) {
        size_t size = first_.size() * 2 + 1;
        while (true) {
            vector<optional<int>> newFirst(size);
            vector<optional<int>> newSecond(size);
            bool complete = true;
            for (int value : values) {
                if (!place(value, newFirst, newSecond)) {
                    complete = false;
                    break;
                }
            }
            if (complete) {
                first_ = std::move(newFirst);
                second_ = std::move(newSecond);
                ++rebuilds_;
                return;
            }
            size = size * 2 + 1;
        }
    }

    vector<optional<int>> first_;
    vector<optional<int>> second_;
    size_t displacements_ = 0;
    size_t rebuilds_ = 0;
};

class RomanHashTable {
public:
    RomanHashTable() : buckets_(23) {
        const pair<int, const char*> values[] = {
            {1000, "M"}, {900, "CM"}, {500, "D"}, {400, "CD"}, {100, "C"}, {90, "XC"},
            {50, "L"}, {40, "XL"}, {10, "X"}, {9, "IX"}, {5, "V"}, {4, "IV"}, {1, "I"}};
        for (const auto& value : values) add(value.first, value.second);
    }

    const string& get(int key) const {
        const auto& bucket = buckets_[positiveModulo(key, buckets_.size())];
        for (const auto& value : bucket) if (value.first == key) return value.second;
        throw runtime_error("roman numeral is absent");
    }

private:
    void add(int key, const string& value) { buckets_[positiveModulo(key, buckets_.size())].push_back({key, value}); }
    vector<list<pair<int, string>>> buckets_;
};

string convertToRoman(int number) {
    RomanHashTable table;
    const int values[] = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1};
    string result;
    for (int value : values) {
        while (number >= value) {
            result += table.get(value);
            number -= value;
        }
    }
    return result;
}

void benchmark(size_t count) {
    const size_t tableSize = count * 3 + 1;
    LinearProbingSet linear(tableSize);
    DoubleHashingSet doubleHashing(tableSize);
    ChainingSet chaining(tableSize);
    CuckooSet cuckoo(tableSize);
    const auto start = chrono::steady_clock::now();
    for (size_t index = 0; index < count; ++index) {
        const int key = static_cast<int>((index * 7919) % (count * 2 + 1));
        linear.insert(key);
        doubleHashing.insert(key);
        chaining.insert(key);
        cuckoo.insert(key);
    }
    const auto elapsed = chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now() - start).count();
    cout << "Количество ключей: " << count << '\n';
    cout << "Линейное опробование, проверок: " << linear.probes() << '\n';
    cout << "Двойное хеширование, проверок: " << doubleHashing.probes() << '\n';
    cout << "Метод цепочек, сравнений: " << chaining.comparisons() << '\n';
    cout << "Хеширование кукушки, вытеснений: " << cuckoo.displacements() << '\n';
    cout << "Хеширование кукушки, перестроений: " << cuckoo.rebuilds() << '\n';
    cout << "Время эксперимента, мкс: " << elapsed << '\n';
}
}

int task6() {
    string mode;
    if (!(cin >> mode)) {
        cerr << "Ошибка: введите число или команду benchmark.\n";
        return 1;
    }
    if (mode == "benchmark") {
        size_t count = 0;
        if (!(cin >> count) || count == 0) {
            cerr << "Ошибка: укажите положительное число ключей.\n";
            return 1;
        }
        benchmark(count);
        return 0;
    }

    try {
        const int number = stoi(mode);
        if (number < 1 || number > 3999) {
            cerr << "Ошибка: число должно быть в диапазоне от 1 до 3999.\n";
            return 1;
        }
        cout << convertToRoman(number) << '\n';
    } catch (const exception&) {
        cerr << "Ошибка: ожидалось целое число.\n";
        return 1;
    }
    return 0;
}
