#include "StringSet.hpp"
#include "tasks.h"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

StringSet setUnion(const StringSet& first, const StringSet& second) {
    StringSet result;
    for (const string& value : first.values()) {
        result.add(value);
    }
    for (const string& value : second.values()) {
        result.add(value);
    }
    return result;
}

StringSet setIntersection(const StringSet& first, const StringSet& second) {
    StringSet result;
    for (const string& value : first.values()) {
        if (second.contains(value)) {
            result.add(value);
        }
    }
    return result;
}

StringSet setDifference(const StringSet& first, const StringSet& second) {
    StringSet result;
    for (const string& value : first.values()) {
        if (!second.contains(value)) {
            result.add(value);
        }
    }
    return result;
}

static StringSet makeSet(const string& line) {
    StringSet set;
    istringstream input(line);
    string value;
    while (input >> value) set.add(value);
    return set;
}

static void printValues(vector<string> values) {
    sort(values.begin(), values.end());
    cout << "{ ";
    for (const string& value : values) cout << value << ' ';
    cout << "}\n";
}

int task3() {
    string firstLine;
    string secondLine;
    getline(cin, firstLine);
    getline(cin, secondLine);
    const StringSet first = makeSet(firstLine);
    const StringSet second = makeSet(secondLine);

    const StringSet unionSet = setUnion(first, second);
    const StringSet intersection = setIntersection(first, second);
    const StringSet difference = setDifference(first, second);

    cout << "Объединение: ";
    printValues(unionSet.values());
    cout << "Пересечение: ";
    printValues(intersection.values());
    cout << "Разность A\\B: ";
    printValues(difference.values());
    return 0;
}
