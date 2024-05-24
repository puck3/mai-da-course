cmake_minimum_required(VERSION 3.28)
project(lw2)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(tests_exe tests-generator.cpp)
add_executable(lw2_exe main.cpp)
