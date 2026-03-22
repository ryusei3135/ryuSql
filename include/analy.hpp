#pragma once

#include <string>
#include <iostream>
#include <cstdint>
#include <vector>
#include <array>
#include <concepts>
#include <utility>
#include <cstring>
#include <expected>
#include <optional>

#include "error.hpp"
#include "token.hpp"


using ReturnTokenKind = std::expected<std::optional<TokenKind>, Errors>;

enum class CharKinds {
    Letter,
    Digit,
    Space,
    Symbol,
    Other,
    NONE,
};

// テーブルの初期化
template<typename T>
concept EnumOnly = std::is_enum_v<T>;

template<EnumOnly T>
consteval const std::array<T, 256> init_char_table() {
    using FillValues = std::vector<std::pair<std::array<char, 2>, T>>;
    auto fill_table = [&](auto& table, std::array<char, 2> range, T value) {
        for (int i = range[0]; i <= range[1]; i++)
            table[i] = value;
    };
    std::array<T, 256> table{};

    if constexpr (std::is_same_v<T, CharKinds>) {
        FillValues fill_values_list = {
            {{(char)0, (char)255}, CharKinds::Other},
            {{(char)9, (char)13}, CharKinds::Space},
            {{'!', '/'}, CharKinds::Symbol},
            {{':', '@'}, CharKinds::Symbol},
            {{'A', 'Z'}, CharKinds::Letter},
            {{'a', 'z'}, CharKinds::Letter},
            {{'0', '9'}, CharKinds::Digit}
        };
        for (auto [range, fill_value]: fill_values_list)
            fill_table(table, range, fill_value);
        table[' '] = CharKinds::Space;
    } else {
        fill_table(table, std::array{(char)0, (char)255}, TokenKind::Null);
        table['('] = TokenKind::LeftParen;
        table[')'] = TokenKind::RightParen;
        table[','] = TokenKind::Comma;
        table[';'] = TokenKind::Semicolon;
    }

    return table;
}

// lexer.cpp
std::expected<std::vector<Token>, Errors>
input_sql_query(const char* ascii_sql_query);