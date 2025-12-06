cmake_minimum_required(VERSION 4.0)
project(drim_lang C)

set(CMAKE_C_STANDARD 11)

include_directories(.)
include_directories(include)

add_executable(drim_lang
        include/Token.h
        include/Token.h
        include/Lexer.h
        src/Lexer.cpp
        include/Utils.h
        src/Utils.cpp)
