#include "structures.h"
#include "tasks.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

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

    StringSet unionSet;
    StringSet intersection;
    StringSet difference;
    for (const string& value : first.values()) {
        unionSet.add(value);
        if (second.contains(value)) {
            intersection.add(value);
        } else {
            difference.add(value);
        }
    }
    for (const string& value : second.values()) unionSet.add(value);

    cout << "Объединение: ";
    printValues(unionSet.values());
    cout << "Пересечение: ";
    printValues(intersection.values());
    cout << "Разность A\\B: ";
    printValues(difference.values());
    return 0;
}
