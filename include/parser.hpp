#pragma once


#include "analy.hpp"
#include "error.hpp"
#include "ast_node.hpp"
#include "RyuSql.hpp"

#include "checks.tpp"

namespace Parser {
    // astのノードの親子関係を構築
    namespace Linker {
        Opt::ErrOpt ast_link(Ast& ast, const size_t left, const size_t right);
    };
    // paren_node.cpp
    std::expected<std::vector<Ast>, Errors> create_paren_node(
        const std::vector<Token>& tokens, 
        size_t* pos,
        const size_t parent_id
    );
    // expr/create_table.cpp
    std::expected<std::vector<Ast>, Errors> 
    create_CREATE_TABLE_node(const std::vector<Token>& tokens, size_t* pos);
    // ast.cpp
    std::expected<std::vector<Ast>, Errors>
    create_ast_node(const std::vector<Token>& tokens);
};