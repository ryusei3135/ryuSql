#pragma once

#include <string>

enum class TokenKind {
    FROM,
    TABLE,
    SELECT,
    CREATE,
    STRING,
    NUMBER,
    INSERT,
    INTO,
    VALUES,
    LeftParen,
    RightParen,
    Comma,
    Semicolon,
    ColumnType,
    Null,
};
struct Token {
    std::string  value;
    TokenKind  token_kind;
};
