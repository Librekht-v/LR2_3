package main

import (
	"fmt"
	"io"
)

func task5(input io.Reader, output io.Writer) error {
	var tree binarySearchTree
	hasValues := false
	for {
		var value int
		if _, err := fmt.Fscan(input, &value); err != nil {
			if !hasValues {
				return fmt.Errorf("дерево не содержит элементов")
			}
			break
		}
		if value == 0 {
			break
		}
		tree.insert(value)
		hasValues = true
	}
	if !hasValues {
		return fmt.Errorf("дерево не содержит элементов")
	}
	if tree.isAVLBalanced() {
		_, err := fmt.Fprintln(output, "YES")
		return err
	}
	_, err := fmt.Fprintln(output, "NO")
	return err
}
