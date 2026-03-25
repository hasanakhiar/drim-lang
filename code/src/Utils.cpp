//
// Created by Muntahi Hasan Akhiar on 6/12/25.
//

#include "../include/Utils.h"

bool isDigit(char c) {
    return c >= '0' && c <= '9';
}

bool isAlpha(char c) {
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           c == '_';
}

bool isAlphaNumeric(char c) {
    return isAlpha(c) || isDigit(c);
}