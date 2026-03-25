#pragma once


#include "analy.hpp"
#include "error.hpp"
#include "ast_node.hpp"
#include "RyuSql.hpp"


using AstNode = std::vector<Ast>;

// astノードを初期化する関数を提供する
#include "ast.tpp"
// tokenの種類を比較し値を返すapiを提供
#include "checks.tpp"

// Nodeはast.cpp
namespace Parser {
    // # ===== ast.cpp ===== #
    // astのノードの親子関係を構築
    namespace AstLinker {
        Opt::ErrOpt ast_link(Ast& ast, const size_t left, const size_t right);
    };
    // paren_node.cpp
    std::expected<AstNode, Errors>
    create_paren_node(const std::vector<Token>& tokens, size_t* pos);
    // expr/create_table.cpp
    std::expected<AstNode, Errors>
    create_CREATE_TABLE_node(const std::vector<Token>& tokens, size_t* pos);
    // ast.cpp
    std::expected<AstNode, Errors>
    create_ast_node(const std::vector<Token>& tokens);
};
