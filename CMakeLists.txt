cmake_minimum_required(VERSION 3.10)
project(drim_lang)

set(CMAKE_CXX_STANDARD 17)

include_directories(code/include)


set(SOURCES
        code/src/Utils.cpp
        code/src/Lexer.cpp
        code/src/Parser.cpp
        code/src/Interpreter.cpp
        code/src/Main.cpp
        code/src/Physics.cpp
        code/src/DS.cpp
)

add_executable(drim ${SOURCES})