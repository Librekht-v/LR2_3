package main

import (
	"fmt"
	"io"
	"os"
	"strings"
)

func readSet(path string) (stringSet, error) {
	result := newStringSet()
	data, err := os.ReadFile(path)
	if os.IsNotExist(err) {
		return result, nil
	}
	if err != nil {
		return result, err
	}
	for _, value := range strings.Split(strings.TrimSpace(string(data)), "\n") {
		value = strings.TrimSpace(value)
		if value != "" {
			result.add(value)
		}
	}
	return result, nil
}

func writeSet(path string, set stringSet) error {
	values := set.sortedValues()
	data := ""
	if len(values) > 0 {
		data = strings.Join(values, "\n") + "\n"
	}
	return os.WriteFile(path, []byte(data), 0644)
}

func task2(arguments []string, output io.Writer) error {
	path := ""
	query := ""
	for index := 0; index < len(arguments); index++ {
		switch arguments[index] {
		case "--file":
			if index+1 < len(arguments) {
				index++
				path = arguments[index]
			}
		case "--query":
			if index+1 < len(arguments) {
				index++
				query = arguments[index]
			}
		}
	}
	if path == "" || query == "" {
		return fmt.Errorf("использование: lab2-go 2 --file data.txt --query \"SETADD значение\"")
	}

	set, err := readSet(path)
	if err != nil {
		return err
	}
	parts := strings.Fields(query)
	if len(parts) != 2 {
		return fmt.Errorf("запрос должен состоять из команды и значения")
	}
	switch parts[0] {
	case "SETADD":
		if set.add(parts[1]) {
			fmt.Fprintln(output, "ADDED")
		} else {
			fmt.Fprintln(output, "ALREADY_EXISTS")
		}
		return writeSet(path, set)
	case "SETDEL":
		if set.erase(parts[1]) {
			fmt.Fprintln(output, "DELETED")
		} else {
			fmt.Fprintln(output, "NOT_FOUND")
		}
		return writeSet(path, set)
	case "SET_AT":
		if set.contains(parts[1]) {
			fmt.Fprintln(output, "TRUE")
		} else {
			fmt.Fprintln(output, "FALSE")
		}
		return nil
	default:
		return fmt.Errorf("неизвестная команда")
	}
}
