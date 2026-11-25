cmake_minimum_required(VERSION 3.20)
project(lab2_variant3 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(lab2
    cpp/src/main.cpp
    cpp/src/task1.cpp
    cpp/src/task2.cpp
    cpp/src/task3.cpp
    cpp/src/task4.cpp
    cpp/src/task5.cpp
    cpp/src/task6.cpp
    cpp/src/task7.cpp)

target_include_directories(lab2 PRIVATE cpp/include)

enable_testing()
add_test(NAME smoke_help COMMAND lab2 --help)
