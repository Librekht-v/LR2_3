package main

import (
	"bufio"
	"fmt"
	"io"
	"strings"
)

func setFromLine(line string) stringSet {
	result := newStringSet()
	for _, value := range strings.Fields(line) {
		result.add(value)
	}
	return result
}

func printSet(output io.Writer, set stringSet) {
	fmt.Fprint(output, "{ ")
	for _, value := range set.sortedValues() {
		fmt.Fprint(output, value, " ")
	}
	fmt.Fprintln(output, "}")
}

func task3(input io.Reader, output io.Writer) error {
	scanner := bufio.NewScanner(input)
	if !scanner.Scan() {
		return fmt.Errorf("не задано первое множество")
	}
	first := setFromLine(scanner.Text())
	if !scanner.Scan() {
		return fmt.Errorf("не задано второе множество")
	}
	second := setFromLine(scanner.Text())
	if err := scanner.Err(); err != nil {
		return err
	}

	union := newStringSet()
	intersection := newStringSet()
	difference := newStringSet()
	for value := range first.values {
		union.add(value)
		if second.contains(value) {
			intersection.add(value)
		} else {
			difference.add(value)
		}
	}
	for value := range second.values {
		union.add(value)
	}
	fmt.Fprint(output, "Объединение: ")
	printSet(output, union)
	fmt.Fprint(output, "Пересечение: ")
	printSet(output, intersection)
	fmt.Fprint(output, "Разность A\\B: ")
	printSet(output, difference)
	return nil
}
