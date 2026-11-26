package main

import (
	"bufio"
	"fmt"
	"io"
	"strings"
	"unicode"
)

func operationPriority(operation rune) int {
	switch operation {
	case '+', '-':
		return 1
	case '*', '/':
		return 2
	case '^':
		return 3
	default:
		return -1
	}
}

func isOperation(symbol rune) bool {
	return strings.ContainsRune("+-*/^", symbol)
}

func infixToPostfix(expression string) (string, error) {
	var operations runeStack
	var tokens []string
	expectsOperand := true
	runes := []rune(expression)

	for index := 0; index < len(runes); {
		symbol := runes[index]
		if unicode.IsSpace(symbol) {
			index++
			continue
		}
		if unicode.IsLetter(symbol) || unicode.IsDigit(symbol) || symbol == '_' {
			if !expectsOperand {
				return "", fmt.Errorf("пропущен оператор")
			}
			start := index
			for index < len(runes) && (unicode.IsLetter(runes[index]) || unicode.IsDigit(runes[index]) || runes[index] == '_') {
				index++
			}
			tokens = append(tokens, string(runes[start:index]))
			expectsOperand = false
			continue
		}
		if symbol == '(' {
			if !expectsOperand {
				return "", fmt.Errorf("пропущен оператор перед скобкой")
			}
			operations.push(symbol)
			index++
			continue
		}
		if symbol == ')' {
			if expectsOperand {
				return "", fmt.Errorf("пустые скобки или лишний оператор")
			}
			for !operations.empty() && operations.top() != '(' {
				tokens = append(tokens, string(operations.pop()))
			}
			if operations.empty() {
				return "", fmt.Errorf("закрывающая скобка без открывающей")
			}
			operations.pop()
			index++
			continue
		}
		if isOperation(symbol) {
			if expectsOperand {
				return "", fmt.Errorf("оператор стоит без левого операнда")
			}
			for !operations.empty() && operations.top() != '(' &&
				(operationPriority(operations.top()) > operationPriority(symbol) ||
					(operationPriority(operations.top()) == operationPriority(symbol) && symbol != '^')) {
				tokens = append(tokens, string(operations.pop()))
			}
			operations.push(symbol)
			expectsOperand = true
			index++
			continue
		}
		return "", fmt.Errorf("недопустимый символ %q", symbol)
	}
	if expectsOperand {
		return "", fmt.Errorf("выражение оканчивается оператором")
	}
	for !operations.empty() {
		if operations.top() == '(' {
			return "", fmt.Errorf("открывающая скобка без закрывающей")
		}
		tokens = append(tokens, string(operations.pop()))
	}
	return strings.Join(tokens, " "), nil
}

func task1(input io.Reader, output io.Writer) error {
	line, err := bufio.NewReader(input).ReadString('\n')
	if err != nil && len(line) == 0 {
		return fmt.Errorf("выражение не задано")
	}
	result, err := infixToPostfix(strings.TrimRight(line, "\r\n"))
	if err != nil {
		return err
	}
	_, err = fmt.Fprintln(output, result)
	return err
}
