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

func TestSetOperations(t *testing.T) {
	a := setFromLine("1 2 3 4")
	b := setFromLine("3 4 5")
	if len(setUnion(a, b).sortedValues()) != 5 {
		t.Fatal("неверное объединение")
	}
	if len(setIntersection(a, b).sortedValues()) != 2 {
		t.Fatal("неверное пересечение")
	}
	if len(setDifference(a, b).sortedValues()) != 2 {
		t.Fatal("неверная разность")
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

func TestHashTablesKeyValueAndDelete(t *testing.T) {
	lin := newLinearHashTable(7)
	dbl := newDoubleHashTable(7)
	for _, k := range []int{1, 8, 15, 22} {
		lin.insert(k, "v"+string(rune('0'+k%10)))
		dbl.insert(k, "v"+string(rune('0'+k%10)))
	}
	if v, ok := lin.find(15); !ok || v != "v5" {
		t.Fatalf("linear find(15): %q %v", v, ok)
	}
	if v, ok := dbl.find(15); !ok || v != "v5" {
		t.Fatalf("double find(15): %q %v", v, ok)
	}
	if !lin.remove(8) || !dbl.remove(8) {
		t.Fatal("ошибка удаления ключа 8")
	}
	if _, ok := lin.find(8); ok {
		t.Fatal("ключ 8 не должен находиться после удаления")
	}
	if v, ok := dbl.find(22); !ok || v != "v2" {
		t.Fatalf("нарушена цепочка проб после удаления: %q %v", v, ok)
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
