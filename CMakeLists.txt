cmake_minimum_required(VERSION 3.10)
project(drim_lang)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)


set(SOURCES
        src/Utils.cpp
        src/Lexer.cpp
        src/Parser.cpp
        src/Interpreter.cpp
        src/Main.cpp
)

add_executable(test ${SOURCES})