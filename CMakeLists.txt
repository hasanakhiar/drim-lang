cmake_minimum_required(VERSION 3.10)
project(drim_lang)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(drim_lang
        src/tests/test_parser.cpp
        src/units/parser.cpp
        src/units/lexer.cpp
        src/units/interpreter.cpp
)