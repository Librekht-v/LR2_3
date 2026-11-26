package main

import (
	"container/list"
	"fmt"
	"io"
)

type lruValue struct {
	value    int
	position *list.Element
}

type lruCache struct {
	capacity int
	order    *list.List
	values   map[int]lruValue
}

func newLRUCache(capacity int) lruCache {
	return lruCache{capacity: capacity, order: list.New(), values: make(map[int]lruValue)}
}

func (c *lruCache) touch(key int, value lruValue) {
	c.order.MoveToFront(value.position)
	c.values[key] = lruValue{value: value.value, position: value.position}
}

func (c *lruCache) set(key int, value int) {
	if found, exists := c.values[key]; exists {
		found.value = value
		c.order.MoveToFront(found.position)
		c.values[key] = found
		return
	}
	if len(c.values) == c.capacity {
		last := c.order.Back()
		delete(c.values, last.Value.(int))
		c.order.Remove(last)
	}
	position := c.order.PushFront(key)
	c.values[key] = lruValue{value: value, position: position}
}

func (c *lruCache) get(key int) int {
	found, exists := c.values[key]
	if !exists {
		return -1
	}
	c.touch(key, found)
	return found.value
}

func task7(input io.Reader, output io.Writer) error {
	var cacheType string
	var capacity int
	var queryCount int
	if _, err := fmt.Fscan(input, &cacheType, &capacity, &queryCount); err != nil || capacity < 1 {
		return fmt.Errorf("формат: LRU, затем ёмкость и число запросов")
	}
	if cacheType != "LRU" {
		return fmt.Errorf("для задания 7 реализован LRU-кэш")
	}
	cache := newLRUCache(capacity)
	for index := 0; index < queryCount; index++ {
		var operation string
		var key int
		if _, err := fmt.Fscan(input, &operation, &key); err != nil {
			return fmt.Errorf("неполный запрос")
		}
		switch operation {
		case "SET":
			var value int
			if _, err := fmt.Fscan(input, &value); err != nil {
				return fmt.Errorf("для SET нужно значение")
			}
			cache.set(key, value)
		case "GET":
			fmt.Fprintln(output, cache.get(key))
		default:
			return fmt.Errorf("неизвестная операция")
		}
	}
	return nil
}
