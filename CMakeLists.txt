cmake_minimum_required(VERSION 4.0)
project(drim_lang)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)


set(SOURCES
        test/Lexer.cpp
        src/Lexer.cpp
        src/Utils.cpp
)

add_executable(test_lexer ${SOURCES})