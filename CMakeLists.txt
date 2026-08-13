cmake_minimum_required(VERSION 3.10)
project(Indexra VERSION 1.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -Wextra -Wpedantic")

# Core library
add_library(indexra_core
    src/tokenizer.cpp
    src/inverted_index.cpp
    src/query_processor.cpp
    src/ranking_engine.cpp
    src/trie.cpp
)

# Executable
add_executable(indexra src/main.cpp)
target_link_libraries(indexra PRIVATE indexra_core)
