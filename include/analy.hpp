#pragma once

#include <string>
#include <iostream>
#include <cstdint>
#include <cstring>
#include <vector>
#include <array>

#include "token.hpp"

enum class CharKinds {
    Letter,
    Digit,
    Space,
    Symbol,
    Other,
    NONE,
};

struct Stack {
    std::string value;
    std::vector<Token> tokens;
    CharKinds last_kind = CharKinds::NONE;
    char last_char = '\0';

    void stack_char(const char chr, CharKinds kind);
    void exit();
};

// lexer.cpp
std::vector<Token> input_sql_query(const char* ascii_sql_query);