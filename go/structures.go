package main

import "sort"

type runeStack struct {
	data []rune
}

func (s *runeStack) push(value rune) {
	s.data = append(s.data, value)
}

func (s *runeStack) pop() rune {
	value := s.data[len(s.data)-1]
	s.data = s.data[:len(s.data)-1]
	return value
}

func (s *runeStack) top() rune {
	return s.data[len(s.data)-1]
}

func (s *runeStack) empty() bool {
	return len(s.data) == 0
}

type stringSet struct {
	values map[string]struct{}
}

func newStringSet() stringSet {
	return stringSet{values: make(map[string]struct{})}
}

func (s stringSet) add(value string) bool {
	if _, exists := s.values[value]; exists {
		return false
	}
	s.values[value] = struct{}{}
	return true
}

func (s stringSet) erase(value string) bool {
	if _, exists := s.values[value]; !exists {
		return false
	}
	delete(s.values, value)
	return true
}

func (s stringSet) contains(value string) bool {
	_, exists := s.values[value]
	return exists
}

func (s stringSet) sortedValues() []string {
	result := make([]string, 0, len(s.values))
	for value := range s.values {
		result = append(result, value)
	}
	sort.Strings(result)
	return result
}

type treeNode struct {
	value int
	left  *treeNode
	right *treeNode
}

type binarySearchTree struct {
	root *treeNode
}

func (t *binarySearchTree) insert(value int) {
	t.root = insertNode(t.root, value)
}

func insertNode(node *treeNode, value int) *treeNode {
	if node == nil {
		return &treeNode{value: value}
	}
	if value < node.value {
		node.left = insertNode(node.left, value)
	} else if value > node.value {
		node.right = insertNode(node.right, value)
	}
	return node
}

func (t binarySearchTree) isAVLBalanced() bool {
	return treeHeight(t.root) >= 0
}

func treeHeight(node *treeNode) int {
	if node == nil {
		return 0
	}
	left := treeHeight(node.left)
	right := treeHeight(node.right)
	if left < 0 || right < 0 || abs(left-right) > 1 {
		return -1
	}
	if left > right {
		return left + 1
	}
	return right + 1
}

func abs(value int) int {
	if value < 0 {
		return -value
	}
	return value
}
