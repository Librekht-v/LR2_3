package main

import (
	"bytes"
	"os"
	"path/filepath"
	"testing"
)

func TestInfixToPostfix(t *testing.T) {
	actual, err := infixToPostfix("(A+B)*C")
	if err != nil || actual != "A B + C *" {
		t.Fatalf("получено %q, ошибка %v", actual, err)
	}
}

func TestWildcardMatch(t *testing.T) {
	if !wildcardMatch("sobaka", "s*ak?") || wildcardMatch("hello", "h?lo") {
		t.Fatal("неверный результат сопоставления с шаблоном")
	}
}

func TestRoman(t *testing.T) {
	if actual := roman(3749); actual != "MMMDCCXLIX" {
		t.Fatalf("получено %q", actual)
	}
}

func TestLRUCache(t *testing.T) {
	cache := newLRUCache(2)
	cache.set(1, 2)
	cache.set(2, 3)
	cache.set(1, 5)
	cache.set(4, 5)
	if cache.get(2) != -1 || cache.get(1) != 5 {
		t.Fatal("кэш удалил неверный элемент")
	}
}

func TestTask2(t *testing.T) {
	path := filepath.Join(t.TempDir(), "data.txt")
	var output bytes.Buffer
	if err := task2([]string{"--file", path, "--query", "SETADD apple"}, &output); err != nil {
		t.Fatal(err)
	}
	data, err := os.ReadFile(path)
	if err != nil || string(data) != "apple\n" {
		t.Fatalf("получены данные %q, ошибка %v", data, err)
	}
}
