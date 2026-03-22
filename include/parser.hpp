#pragma once


#include "analy.hpp"
#include "error.hpp"
#include "ast_node.hpp"

#include "paren_node.tpp"

namespace Parser {
    class Match {
    public:
        Match(
            const std::vector<Token>& ref_tokens
        ) : tokens(ref_tokens) {}

        bool consume(const TokenKind kind, size_t* i);

        std::optional<Ast> expect(
            const TokenKind kind, 
            const AstNodeKind node_kind, 
            size_t* pos
        );
    private:
        std::vector<Token> tokens;
    };

    // paren_node.cpp
    std::optional<std::vector<Ast>> create_paren_node(
        const std::vector<Token>& tokens, 
        size_t* pos
    );
    // expr/create_table.cpp
    std::expected<std::vector<Ast>, Errors> 
    create_CREATE_TABLE_node(const std::vector<Token>& tokens, size_t* pos);
    // ast.cpp
    std::expected<std::vector<Ast>, Errors>
    create_ast_node(const std::vector<Token>& tokens);
};