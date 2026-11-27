package main

import (
	"fmt"
	"io"
	"strconv"
	"time"
)

type slotState int

const (
	slotEmpty slotState = iota
	slotOccupied
	slotDeleted
)

type hashEntry struct {
	state slotState
	key   int
	value string
}

func modulo(value int, size int) int {
	result := value % size
	if result < 0 {
		return result + size
	}
	return result
}

func nextPrime(minCap int) int {
	isPrime := func(n int) bool {
		if n < 2 {
			return false
		}
		for d := 2; d*d <= n; d++ {
			if n%d == 0 {
				return false
			}
		}
		return true
	}
	candidate := minCap
	for !isPrime(candidate) {
		candidate++
	}
	return candidate
}

type linearHashTable struct {
	cells  []hashEntry
	size   int
	probes int
}

func newLinearHashTable(capacity int) linearHashTable {
	if capacity < 2 {
		capacity = 2
	}
	return linearHashTable{cells: make([]hashEntry, capacity)}
}

func (t *linearHashTable) insert(key int, value string) bool {
	cap := len(t.cells)
	start := modulo(key, cap)
	firstDeleted := -1
	for attempt := 0; attempt < cap; attempt++ {
		index := (start + attempt) % cap
		t.probes++
		cell := &t.cells[index]
		if cell.state == slotEmpty {
			target := index
			if firstDeleted != -1 {
				target = firstDeleted
			}
			t.cells[target] = hashEntry{state: slotOccupied, key: key, value: value}
			t.size++
			return true
		}
		if cell.state == slotDeleted {
			if firstDeleted == -1 {
				firstDeleted = index
			}
		} else if cell.key == key {
			cell.value = value
			return false
		}
	}
	if firstDeleted != -1 {
		t.cells[firstDeleted] = hashEntry{state: slotOccupied, key: key, value: value}
		t.size++
		return true
	}
	return false
}

func (t *linearHashTable) find(key int) (string, bool) {
	cap := len(t.cells)
	start := modulo(key, cap)
	for attempt := 0; attempt < cap; attempt++ {
		index := (start + attempt) % cap
		t.probes++
		cell := t.cells[index]
		if cell.state == slotEmpty {
			return "", false
		}
		if cell.state == slotOccupied && cell.key == key {
			return cell.value, true
		}
	}
	return "", false
}

func (t *linearHashTable) remove(key int) bool {
	cap := len(t.cells)
	start := modulo(key, cap)
	for attempt := 0; attempt < cap; attempt++ {
		index := (start + attempt) % cap
		t.probes++
		cell := &t.cells[index]
		if cell.state == slotEmpty {
			return false
		}
		if cell.state == slotOccupied && cell.key == key {
			cell.state = slotDeleted
			cell.value = ""
			t.size--
			return true
		}
	}
	return false
}

type doubleHashTable struct {
	cells  []hashEntry
	size   int
	probes int
}

func newDoubleHashTable(capacity int) doubleHashTable {
	if capacity < 3 {
		capacity = 3
	}
	return doubleHashTable{cells: make([]hashEntry, nextPrime(capacity))}
}

func (t *doubleHashTable) secondHash(key int) int {
	return 1 + modulo(key*37+17, len(t.cells)-1)
}

func (t *doubleHashTable) insert(key int, value string) bool {
	cap := len(t.cells)
	start := modulo(key, cap)
	step := t.secondHash(key)
	firstDeleted := -1
	for attempt := 0; attempt < cap; attempt++ {
		index := (start + attempt*step) % cap
		t.probes++
		cell := &t.cells[index]
		if cell.state == slotEmpty {
			target := index
			if firstDeleted != -1 {
				target = firstDeleted
			}
			t.cells[target] = hashEntry{state: slotOccupied, key: key, value: value}
			t.size++
			return true
		}
		if cell.state == slotDeleted {
			if firstDeleted == -1 {
				firstDeleted = index
			}
		} else if cell.key == key {
			cell.value = value
			return false
		}
	}
	if firstDeleted != -1 {
		t.cells[firstDeleted] = hashEntry{state: slotOccupied, key: key, value: value}
		t.size++
		return true
	}
	return false
}

func (t *doubleHashTable) find(key int) (string, bool) {
	cap := len(t.cells)
	start := modulo(key, cap)
	step := t.secondHash(key)
	for attempt := 0; attempt < cap; attempt++ {
		index := (start + attempt*step) % cap
		t.probes++
		cell := t.cells[index]
		if cell.state == slotEmpty {
			return "", false
		}
		if cell.state == slotOccupied && cell.key == key {
			return cell.value, true
		}
	}
	return "", false
}

func (t *doubleHashTable) remove(key int) bool {
	cap := len(t.cells)
	start := modulo(key, cap)
	step := t.secondHash(key)
	for attempt := 0; attempt < cap; attempt++ {
		index := (start + attempt*step) % cap
		t.probes++
		cell := &t.cells[index]
		if cell.state == slotEmpty {
			return false
		}
		if cell.state == slotOccupied && cell.key == key {
			cell.state = slotDeleted
			cell.value = ""
			t.size--
			return true
		}
	}
	return false
}

func roman(number int) string {
	table := newDoubleHashTable(29)
	entries := []struct {
		key   int
		value string
	}{
		{1000, "M"}, {900, "CM"}, {500, "D"}, {400, "CD"}, {100, "C"}, {90, "XC"},
		{50, "L"}, {40, "XL"}, {10, "X"}, {9, "IX"}, {5, "V"}, {4, "IV"}, {1, "I"},
	}
	for _, entry := range entries {
		table.insert(entry.key, entry.value)
	}
	result := ""
	for _, entry := range entries {
		for number >= entry.key {
			symbol, _ := table.find(entry.key)
			result += symbol
			number -= entry.key
		}
	}
	return result
}

func runBenchmark(count int, output io.Writer) {
	capacity := count + count/3 + 1
	keys := make([]int, count)
	for index := 0; index < count; index++ {
		x := uint32(index + 1)
		x = ((x >> 16) ^ x) * 0x45d9f3b
		x = ((x >> 16) ^ x) * 0x45d9f3b
		x = (x >> 16) ^ x
		keys[index] = int(x & 0x7fffffff)
	}

	linear := newLinearHashTable(capacity)
	lStart := time.Now()
	linear.probes = 0
	for _, key := range keys {
		linear.insert(key, "val_"+strconv.Itoa(key))
	}
	lInsert := linear.probes
	linear.probes = 0
	for _, key := range keys {
		linear.find(key)
	}
	lFind := linear.probes
	linear.probes = 0
	for index := 0; index < len(keys); index += 2 {
		linear.remove(keys[index])
	}
	lRemove := linear.probes
	lUs := time.Since(lStart).Microseconds()

	doubleHashing := newDoubleHashTable(capacity)
	dStart := time.Now()
	doubleHashing.probes = 0
	for _, key := range keys {
		doubleHashing.insert(key, "val_"+strconv.Itoa(key))
	}
	dInsert := doubleHashing.probes
	doubleHashing.probes = 0
	for _, key := range keys {
		doubleHashing.find(key)
	}
	dFind := doubleHashing.probes
	doubleHashing.probes = 0
	for index := 0; index < len(keys); index += 2 {
		doubleHashing.remove(keys[index])
	}
	dRemove := doubleHashing.probes
	dUs := time.Since(dStart).Microseconds()

	fmt.Fprintln(output, "Количество ключей:", count)
	fmt.Fprintf(output, "Линейное опробование (вставка / поиск / удаление, проб): %d / %d / %d, время: %d мкс\n", lInsert, lFind, lRemove, lUs)
	fmt.Fprintf(output, "Двойное хеширование (вставка / поиск / удаление, проб): %d / %d / %d, время: %d мкс\n", dInsert, dFind, dRemove, dUs)
}

func task6(input io.Reader, output io.Writer) error {
	var mode string
	if _, err := fmt.Fscan(input, &mode); err != nil {
		return fmt.Errorf("введите число или команду benchmark")
	}
	if mode == "benchmark" {
		var count int
		if _, err := fmt.Fscan(input, &count); err != nil || count < 1 {
			return fmt.Errorf("укажите положительное число ключей")
		}
		runBenchmark(count, output)
		return nil
	}
	number, err := strconv.Atoi(mode)
	if err != nil || number < 1 || number > 3999 {
		return fmt.Errorf("число должно быть в диапазоне от 1 до 3999")
	}
	_, err = fmt.Fprintln(output, roman(number))
	return err
}
