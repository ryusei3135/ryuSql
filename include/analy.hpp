#pragma once


#include "error.hpp"
#include "token.hpp"
#include "ast_node.hpp"
#include "RyuSql.hpp"

using ReturnTokenKind = std::expected<std::optional<TokenKind>, Errors>;


namespace Analy {
    enum class CharKinds {
        Letter,
        Digit,
        Space,
        Symbol,
        Other,
        NONE,
    };

    namespace AstMake {
        Ast Make(const size_t value, const AstNodeKind K);
    };

    // lexer.cpp
    std::expected<std::vector<Token>, Errors>
    input_sql_query(const char* ascii_sql_query);
};