#!/usr/bin/env bash
set -euo pipefail

root="$(cd "$(dirname "$0")/.." && pwd)"
mkdir -p "$root/build"
g++ -std=c++17 -Wall -Wextra -Werror -pedantic "$root"/cpp/src/*.cpp -I"$root/cpp/include" -o "$root/build/lab2"

expect() {
    local actual="$1"
    local expected="$2"
    local name="$3"
    if [[ "$actual" != "$expected" ]]; then
        echo "FAILED: $name" >&2
        echo "expected: $expected" >&2
        echo "actual:   $actual" >&2
        exit 1
    fi
    echo "PASSED: $name"
}

expect "$(printf '%s\n' 'A+B*C' | "$root/build/lab2" 1)" "A B C * +" "task 1"
expect "$(printf '%s\n%s\n' '1 2 3 4' '3 4 5' | "$root/build/lab2" 3)" $'Объединение: { 1 2 3 4 5 }\nПересечение: { 3 4 }\nРазность A\\B: { 1 2 }' "task 3"
expect "$(printf '%s\n%s\n' 'meow@stud.nstu.ru' '*@stud.nstu.ru' | "$root/build/lab2" 4)" "Соответствует" "task 4"
expect "$(printf '%s\n' '7 3 2 1 9 5 4 6 8 0' | "$root/build/lab2" 5)" "YES" "task 5"
expect "$(printf '%s\n' '3749' | "$root/build/lab2" 6)" "MMMDCCXLIX" "task 6"
expect "$(printf '%s\n' 'LRU 2 8
SET 1 2
SET 2 3
SET 1 5
SET 4 5
SET 6 7
GET 4
SET 1 2
GET 3' | "$root/build/lab2" 7)" $'5\n-1' "task 7"

set_file="$(mktemp)"
expect "$("$root/build/lab2" 2 --file "$set_file" --query 'SETADD apple')" "ADDED" "task 2 add"
expect "$("$root/build/lab2" 2 --file "$set_file" --query 'SET_AT apple')" "TRUE" "task 2 contains"
rm -f "$set_file"

echo "All checks passed."
