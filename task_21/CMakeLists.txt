cmake_minimum_required(VERSION 3.0)
project(BinomialHeap)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 11)

add_executable(heap ${PROJECT_SOURCE_DIR}/binomial_heap.hpp ${PROJECT_SOURCE_DIR}/main.cpp)
target_compile_options(heap PRIVATE -fsanitize=address)
target_link_options(heap PRIVATE -fsanitize=address)
