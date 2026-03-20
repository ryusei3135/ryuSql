#pragma once

#include "analy.hpp"
#include "error.hpp"


enum class NodeKind {
    Start,
    End,
    CreateTable,
    ColumnName,
    ColumnType,
    TableName,
    InsertValues,
    InsertColumns,
    Select,
};

struct AstNode {
    size_t value;
    NodeKind kind;
    std::optional<std::vector<size_t>> children;

    static AstNode make(size_t value, NodeKind kind) {
        return AstNode {
            .value = value,
            .kind = kind,
            .children = std::nullopt
        };
    }
};

using Ast = std::vector<AstNode>;

// create.cpp
auto create_CREATE_TABLE_node(
    std::vector<Token>& tokens, 
    size_t* i
) -> std::expected<Ast, Errors>;
// select.cpp
auto create_SELECT_node(
    std::vector<Token>& tokens, 
    size_t* i
) -> std::expected<Ast, Errors>;
// insert.cpp
std::expected<Ast, Errors> create_INSERT_node(
    std::vector<Token> tokens, 
    size_t* i
);

std::expected<Ast, Errors> create_ast(std::vector<Token>& tokens);