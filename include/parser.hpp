#pragma once

#include <vector>
#include <iostream>
#include <cstdint>
#include <variant>
#include <optional>
#include <type_traits>
#include <expected>
#include <memory>
#include <utility>
#include <algorithm>

#include "token.hpp"
#include "errs.hpp"


enum class NodeKind {
    Table,
    TableName,
    Column,
    ColumnName,
    ColumnType,
    SelectColumn,
    SelectTable,
};

struct AstNode {
    NodeKind kind;
    std::optional<uint32_t> value;
    std::vector<uint32_t> children;
};

using Ast = std::vector<AstNode>;


struct [[nodiscard]] AstPush {
    Ast ast;

    template<typename RetValue>
    RetValue push_ast(
        std::optional<uint32_t> index,
        NodeKind kind
    ) {
        uint32_t id = ast.size();

        ast.push_back(
            AstNode{
                .kind = kind,
                .value = index,
                .children = {}
            }
        );

        if constexpr (!std::is_same_v<RetValue, void>) {
            return id;
        }
    }

    void add_child_id(
        uint32_t parent_id, 
        uint32_t child_id
    ) {
        this->ast[parent_id]
            .children
            .push_back(child_id);
    }

    Ast get_ast() noexcept {
        return std::move(this->ast);
    }
};

// exprs/create.cpp
std::expected<Ast, Errors> create_expr_node(const std::vector<Token>& tokens);
// exprs/select.cpp
std::expected<Ast, Errors> select_expr_node(std::vector<Token> tokens);

// ast.cpp
std::expected<Ast, Errors> ganarate_ast(const std::vector<Token>& tokens);