#pragma once

#include "analy.hpp"
#include "error.hpp"


enum class NodeKind {
    Start,
    End,
    CreateTable,
    ColumnName,
    ColumnType,
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

auto create_CREATE_TABLE_node(
    std::vector<Token>& tokens, 
    size_t* i
) -> std::expected<Ast, Errors>;

std::optional<Errors> create_ast(Ast* ast, std::vector<Token>& tokens);