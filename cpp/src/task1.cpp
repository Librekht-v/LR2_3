#include "StackStructure.hpp"
#include "tasks.h"

#include <cctype>
#include <iostream>
#include <string>

using namespace std;

static int priority(char operation) {
    if (operation == '+' || operation == '-') return 1;
    if (operation == '*' || operation == '/') return 2;
    if (operation == '^') return 3;
    return -1;
}

static bool isOperator(char symbol) {
    return symbol == '+' || symbol == '-' || symbol == '*' || symbol == '/' || symbol == '^';
}

int task1() {
    string expression;
    getline(cin, expression);
    if (expression.empty()) {
        cerr << "Ошибка: выражение не задано.\n";
        return 1;
    }

    StackStructure operations;
    string result;
    bool expectsOperand = true;

    for (size_t index = 0; index < expression.size();) {
        const char symbol = expression[index];
        if (isspace(static_cast<unsigned char>(symbol))) {
            ++index;
            continue;
        }

        if (isalnum(static_cast<unsigned char>(symbol)) || symbol == '_') {
            if (!expectsOperand) {
                cerr << "Ошибка: пропущен оператор.\n";
                return 1;
            }
            const size_t start = index;
            while (index < expression.size() &&
                   (isalnum(static_cast<unsigned char>(expression[index])) || expression[index] == '_')) {
                ++index;
            }
            result += expression.substr(start, index - start) + ' ';
            expectsOperand = false;
            continue;
        }

        if (symbol == '(') {
            if (!expectsOperand) {
                cerr << "Ошибка: пропущен оператор перед скобкой.\n";
                return 1;
            }
            operations.push(string(1, symbol));
            ++index;
            continue;
        }

        if (symbol == ')') {
            if (expectsOperand) {
                cerr << "Ошибка: пустые скобки или лишний оператор.\n";
                return 1;
            }
            while (!operations.empty() && operations.top()[0] != '(') {
                result += operations.pop();
                result += ' ';
            }
            if (operations.empty()) {
                cerr << "Ошибка: закрывающая скобка без открывающей.\n";
                return 1;
            }
            operations.pop();
            ++index;
            continue;
        }

        if (isOperator(symbol)) {
            if (expectsOperand) {
                cerr << "Ошибка: оператор стоит без левого операнда.\n";
                return 1;
            }
            while (!operations.empty() && operations.top()[0] != '(' &&
                   (priority(operations.top()[0]) > priority(symbol) ||
                    (priority(operations.top()[0]) == priority(symbol) && symbol != '^'))) {
                result += operations.pop();
                result += ' ';
            }
            operations.push(string(1, symbol));
            expectsOperand = true;
            ++index;
            continue;
        }

        cerr << "Ошибка: недопустимый символ '" << symbol << "'.\n";
        return 1;
    }

    if (expectsOperand) {
        cerr << "Ошибка: выражение оканчивается оператором.\n";
        return 1;
    }
    while (!operations.empty()) {
        if (operations.top()[0] == '(') {
            cerr << "Ошибка: открывающая скобка без закрывающей.\n";
            return 1;
        }
        result += operations.pop();
        result += ' ';
    }

    if (!result.empty()) result.pop_back();
    cout << result << '\n';
    return 0;
}
