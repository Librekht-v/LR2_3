#include "tasks.h"

#include <iostream>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 2 || string(argv[1]) == "--help") {
        cout << "Использование: lab2 <1..7> [параметры]\n";
        cout << "Задание 2: lab2 2 --file data.txt --query \"SETADD значение\"\n";
        return argc < 2 ? 1 : 0;
    }

    const string task = argv[1];
    if (task == "1") return task1();
    if (task == "2") return task2(argc - 1, argv + 1);
    if (task == "3") return task3();
    if (task == "4") return task4();
    if (task == "5") return task5();
    if (task == "6") return task6();
    if (task == "7") return task7();
    cerr << "Ошибка: номер задания должен быть от 1 до 7.\n";
    return 1;
}
