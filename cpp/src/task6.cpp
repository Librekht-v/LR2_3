#include "DoubleHashingHashTable.hpp"
#include "LinearProbingHashTable.hpp"
#include "tasks.h"

#include <chrono>
#include <cstdint>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

using namespace std;

static DoubleHashingHashTable buildRomanTable() {
    DoubleHashingHashTable table(29);
    const pair<int, const char*> values[] = {
        {1000, "M"}, {900, "CM"}, {500, "D"}, {400, "CD"}, {100, "C"}, {90, "XC"},
        {50, "L"}, {40, "XL"}, {10, "X"}, {9, "IX"}, {5, "V"}, {4, "IV"}, {1, "I"}
    };
    for (const auto& entry : values) {
        table.insert(entry.first, entry.second);
    }
    return table;
}

static string convertToRoman(int number) {
    const DoubleHashingHashTable table = buildRomanTable();
    const int values[] = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1};
    string result;
    for (int value : values) {
        while (number >= value) {
            const optional<string> symbol = table.find(value);
            if (!symbol.has_value()) {
                throw runtime_error("roman numeral is absent");
            }
            result += *symbol;
            number -= value;
        }
    }
    return result;
}

template <typename HashTable>
static long long runTableBenchmark(size_t count, size_t capacity, size_t& insertProbes, size_t& findProbes, size_t& removeProbes) {
    HashTable table(capacity);
    vector<int> keys;
    keys.reserve(count);
    for (size_t index = 0; index < count; ++index) {
        uint32_t x = static_cast<uint32_t>(index + 1);
        x = ((x >> 16) ^ x) * 0x45d9f3bu;
        x = ((x >> 16) ^ x) * 0x45d9f3bu;
        x = (x >> 16) ^ x;
        keys.push_back(static_cast<int>(x & 0x7fffffffu));
    }

    const auto start = chrono::steady_clock::now();

    table.resetProbes();
    for (int key : keys) {
        table.insert(key, "val_" + to_string(key));
    }
    insertProbes = table.probes();

    table.resetProbes();
    for (int key : keys) {
        const optional<string> value = table.find(key);
        if (!value.has_value() || *value != "val_" + to_string(key)) {
            throw runtime_error("hash table find returned unexpected value");
        }
    }
    findProbes = table.probes();

    table.resetProbes();
    for (size_t index = 0; index < keys.size(); index += 2) {
        table.remove(keys[index]);
    }
    removeProbes = table.probes();

    const long long elapsed = chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now() - start).count();

    for (size_t index = 0; index < keys.size(); index += 2) {
        if (table.find(keys[index]).has_value()) {
            throw runtime_error("removed key still found in hash table");
        }
    }
    for (size_t index = 1; index < keys.size(); index += 2) {
        const optional<string> value = table.find(keys[index]);
        if (!value.has_value() || *value != "val_" + to_string(keys[index])) {
            throw runtime_error("surviving key lost after remove");
        }
    }

    return elapsed;
}

static void benchmark(size_t count) {
    const size_t capacity = count + count / 3 + 1;

    size_t linearInsert = 0;
    size_t linearFind = 0;
    size_t linearRemove = 0;
    const long long linearUs = runTableBenchmark<LinearProbingHashTable>(
        count, capacity, linearInsert, linearFind, linearRemove);

    size_t doubleInsert = 0;
    size_t doubleFind = 0;
    size_t doubleRemove = 0;
    const long long doubleUs = runTableBenchmark<DoubleHashingHashTable>(
        count, capacity, doubleInsert, doubleFind, doubleRemove);

    cout << "Количество ключей: " << count << '\n';
    cout << "Линейное опробование (вставка / поиск / удаление, проб): "
         << linearInsert << " / " << linearFind << " / " << linearRemove
         << ", время: " << linearUs << " мкс\n";
    cout << "Двойное хеширование (вставка / поиск / удаление, проб): "
         << doubleInsert << " / " << doubleFind << " / " << doubleRemove
         << ", время: " << doubleUs << " мкс\n";
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
