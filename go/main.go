package main

import (
	"fmt"
	"os"
)

func main() {
	if len(os.Args) < 2 || os.Args[1] == "--help" {
		fmt.Println("Использование: lab2-go <1..7> [параметры]")
		fmt.Println("Задание 2: lab2-go 2 --file data.txt --query \"SETADD значение\"")
		if len(os.Args) < 2 {
			os.Exit(1)
		}
		return
	}

	var err error
	switch os.Args[1] {
	case "1":
		err = task1(os.Stdin, os.Stdout)
	case "2":
		err = task2(os.Args[2:], os.Stdout)
	case "3":
		err = task3(os.Stdin, os.Stdout)
	case "4":
		err = task4(os.Stdin, os.Stdout)
	case "5":
		err = task5(os.Stdin, os.Stdout)
	case "6":
		err = task6(os.Stdin, os.Stdout)
	case "7":
		err = task7(os.Stdin, os.Stdout)
	default:
		err = fmt.Errorf("номер задания должен быть от 1 до 7")
	}
	if err != nil {
		fmt.Fprintln(os.Stderr, "Ошибка:", err)
		os.Exit(1)
	}
}
