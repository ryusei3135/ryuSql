#pragma once

#include <string>

enum class TokenKind {
    FROM,
    TABLE,
    SELECT,
    CREATE,
    STRING,
    NUMBER,
    LeftParen,
    RightParen,
    Comma,
    Semicolon,
    ColumnType,
    Null,
};

struct Token {
    std::string value;
    TokenKind token_kind;
};