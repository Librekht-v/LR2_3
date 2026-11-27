package main

import (
	"bufio"
	"fmt"
	"io"
	"strings"
)

func setUnion(first stringSet, second stringSet) stringSet {
	result := newStringSet()
	for value := range first.values {
		result.add(value)
	}
	for value := range second.values {
		result.add(value)
	}
	return result
}

func setIntersection(first stringSet, second stringSet) stringSet {
	result := newStringSet()
	for value := range first.values {
		if second.contains(value) {
			result.add(value)
		}
	}
	return result
}

func setDifference(first stringSet, second stringSet) stringSet {
	result := newStringSet()
	for value := range first.values {
		if !second.contains(value) {
			result.add(value)
		}
	}
	return result
}

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

	union := setUnion(first, second)
	intersection := setIntersection(first, second)
	difference := setDifference(first, second)

	fmt.Fprint(output, "Объединение: ")
	printSet(output, union)
	fmt.Fprint(output, "Пересечение: ")
	printSet(output, intersection)
	fmt.Fprint(output, "Разность A\\B: ")
	printSet(output, difference)
	return nil
}
