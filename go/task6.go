package main

import (
	"fmt"
	"io"
	"strconv"
	"time"
)

func modulo(value int, size int) int {
	result := value % size
	if result < 0 {
		return result + size
	}
	return result
}

type linearSet struct {
	cells  []*int
	probes int
}

func newLinearSet(size int) linearSet {
	return linearSet{cells: make([]*int, size)}
}

func (s *linearSet) insert(key int) bool {
	for attempt := 0; attempt < len(s.cells); attempt++ {
		index := (modulo(key, len(s.cells)) + attempt) % len(s.cells)
		s.probes++
		if s.cells[index] == nil {
			value := key
			s.cells[index] = &value
			return true
		}
		if *s.cells[index] == key {
			return false
		}
	}
	return false
}

type doubleHashSet struct {
	cells  []*int
	probes int
}

func newDoubleHashSet(size int) doubleHashSet {
	return doubleHashSet{cells: make([]*int, size)}
}

func (s *doubleHashSet) insert(key int) bool {
	first := modulo(key, len(s.cells))
	step := 1 + modulo(key*37+17, len(s.cells)-1)
	for attempt := 0; attempt < len(s.cells); attempt++ {
		index := (first + attempt*step) % len(s.cells)
		s.probes++
		if s.cells[index] == nil {
			value := key
			s.cells[index] = &value
			return true
		}
		if *s.cells[index] == key {
			return false
		}
	}
	return false
}

type chainingSet struct {
	buckets     [][]int
	comparisons int
}

func newChainingSet(size int) chainingSet {
	return chainingSet{buckets: make([][]int, size)}
}

func (s *chainingSet) insert(key int) bool {
	index := modulo(key, len(s.buckets))
	for _, value := range s.buckets[index] {
		s.comparisons++
		if value == key {
			return false
		}
	}
	s.buckets[index] = append(s.buckets[index], key)
	return true
}

type cuckooSet struct {
	first         []*int
	second        []*int
	displacements int
	rebuilds      int
}

func newCuckooSet(size int) cuckooSet {
	return cuckooSet{first: make([]*int, size), second: make([]*int, size)}
}

func firstHash(key int, size int) int {
	return modulo(key, size)
}

func secondHash(key int, size int) int {
	return modulo(key*53+19, size)
}

func (s *cuckooSet) contains(key int) bool {
	first := s.first[firstHash(key, len(s.first))]
	second := s.second[secondHash(key, len(s.second))]
	return (first != nil && *first == key) || (second != nil && *second == key)
}

func (s *cuckooSet) place(key int, first []*int, second []*int) bool {
	current := key
	useFirst := true
	for attempt := 0; attempt < len(first)*2; attempt++ {
		var table []*int
		var index int
		if useFirst {
			table = first
			index = firstHash(current, len(table))
		} else {
			table = second
			index = secondHash(current, len(table))
		}
		if table[index] == nil {
			value := current
			table[index] = &value
			return true
		}
		current, *table[index] = *table[index], current
		s.displacements++
		useFirst = !useFirst
	}
	return false
}

func (s *cuckooSet) values() []int {
	result := make([]int, 0)
	for _, value := range s.first {
		if value != nil {
			result = append(result, *value)
		}
	}
	for _, value := range s.second {
		if value != nil {
			result = append(result, *value)
		}
	}
	return result
}

func (s *cuckooSet) rebuild(values []int) {
	size := len(s.first)*2 + 1
	for {
		first := make([]*int, size)
		second := make([]*int, size)
		complete := true
		for _, value := range values {
			if !s.place(value, first, second) {
				complete = false
				break
			}
		}
		if complete {
			s.first = first
			s.second = second
			s.rebuilds++
			return
		}
		size = size*2 + 1
	}
}

func (s *cuckooSet) insert(key int) bool {
	if s.contains(key) {
		return false
	}
	if !s.place(key, s.first, s.second) {
		values := s.values()
		values = append(values, key)
		s.rebuild(values)
	}
	return true
}

func roman(number int) string {
	table := map[int]string{1000: "M", 900: "CM", 500: "D", 400: "CD", 100: "C", 90: "XC", 50: "L", 40: "XL", 10: "X", 9: "IX", 5: "V", 4: "IV", 1: "I"}
	values := []int{1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1}
	result := ""
	for _, value := range values {
		for number >= value {
			result += table[value]
			number -= value
		}
	}
	return result
}

func runBenchmark(count int, output io.Writer) {
	tableSize := count*3 + 1
	linear := newLinearSet(tableSize)
	doubleHashing := newDoubleHashSet(tableSize)
	chaining := newChainingSet(tableSize)
	cuckoo := newCuckooSet(tableSize)
	start := time.Now()
	for index := 0; index < count; index++ {
		key := (index * 7919) % (count*2 + 1)
		linear.insert(key)
		doubleHashing.insert(key)
		chaining.insert(key)
		cuckoo.insert(key)
	}
	fmt.Fprintln(output, "Количество ключей:", count)
	fmt.Fprintln(output, "Линейное опробование, проверок:", linear.probes)
	fmt.Fprintln(output, "Двойное хеширование, проверок:", doubleHashing.probes)
	fmt.Fprintln(output, "Метод цепочек, сравнений:", chaining.comparisons)
	fmt.Fprintln(output, "Хеширование кукушки, вытеснений:", cuckoo.displacements)
	fmt.Fprintln(output, "Хеширование кукушки, перестроений:", cuckoo.rebuilds)
	fmt.Fprintln(output, "Время эксперимента, мкс:", time.Since(start).Microseconds())
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
