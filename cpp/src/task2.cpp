#include "StringSet.hpp"
#include "tasks.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

static bool readSetFile(const string& path, StringSet& set) {
    ifstream input(path);
    if (!input) return false;
    string value;
    while (getline(input, value)) {
        if (!value.empty()) set.add(value);
    }
    return true;
}
// set_at
static bool saveSetFile(const string& path, const StringSet& set) {
    ofstream output(path);
    if (!output) return false;
    vector<string> values = set.values();
    sort(values.begin(), values.end());
    for (const string& value : values) output << value << '\n';
    return true;
}
//setadd
int task2(int argc, char* argv[]) {
    string path;
    string query;
    for (int index = 1; index < argc; ++index) {
        const string argument = argv[index];
        if (argument == "--file" && index + 1 < argc) path = argv[++index];
        else if (argument == "--query" && index + 1 < argc) query = argv[++index];
    }
    if (path.empty() || query.empty()) {
        cerr << "Использование: lab2 2 --file data.txt --query \"SETADD значение\"\n";
        return 1;
    }

    StringSet set;
    readSetFile(path, set);
    istringstream request(query);
    string command;
    string value;
    if (!(request >> command >> value) || request.rdbuf()->in_avail() != 0) {
        cerr << "Ошибка: запрос должен иметь вид SETADD, SETDEL или SET_AT и значение.\n";
        return 1;
    }

    if (command == "SETADD") {
        cout << (set.add(value) ? "ADDED" : "ALREADY_EXISTS") << '\n';
        if (!saveSetFile(path, set)) {
            cerr << "Ошибка: не удалось записать файл.\n";
            return 1;
        }
    } else if (command == "SETDEL") {
        cout << (set.erase(value) ? "DELETED" : "NOT_FOUND") << '\n';
        if (!saveSetFile(path, set)) {
            cerr << "Ошибка: не удалось записать файл.\n";
            return 1;
        }
    } else if (command == "SET_AT") {
        cout << (set.contains(value) ? "TRUE" : "FALSE") << '\n';
    } else {
        cerr << "Ошибка: неизвестная команда.\n";
        return 1;
    }
    return 0;
}
