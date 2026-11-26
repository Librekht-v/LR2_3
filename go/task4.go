package main

import (
	"bufio"
	"fmt"
	"io"
	"strings"
)

func wildcardMatch(text string, pattern string) bool {
	textRunes := []rune(text)
	patternRunes := []rune(pattern)
	previous := make([]bool, len(patternRunes)+1)
	current := make([]bool, len(patternRunes)+1)
	previous[0] = true
	for index := 1; index <= len(patternRunes); index++ {
		previous[index] = previous[index-1] && patternRunes[index-1] == '*'
	}
	for _, symbol := range textRunes {
		for index := range current {
			current[index] = false
		}
		for column := 1; column <= len(patternRunes); column++ {
			mask := patternRunes[column-1]
			if mask == '*' {
				current[column] = current[column-1] || previous[column]
			} else if mask == '?' || mask == symbol {
				current[column] = previous[column-1]
			}
		}
		previous, current = current, previous
	}
	return previous[len(patternRunes)]
}

func task4(input io.Reader, output io.Writer) error {
	scanner := bufio.NewScanner(input)
	if !scanner.Scan() {
		return fmt.Errorf("не задана последовательность")
	}
	text := scanner.Text()
	if !scanner.Scan() {
		return fmt.Errorf("не задан шаблон")
	}
	pattern := strings.TrimRight(scanner.Text(), "\r\n")
	if err := scanner.Err(); err != nil {
		return err
	}
	if wildcardMatch(text, pattern) {
		_, err := fmt.Fprintln(output, "Соответствует")
		return err
	}
	_, err := fmt.Fprintln(output, "Не соответствует")
	return err
}
