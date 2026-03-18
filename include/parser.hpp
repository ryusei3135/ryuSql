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
    Select,
    SelectColumn,
    SelectTable,
    Insert,
    InsertColumn,
    InsertValues,
};

enum class ExprKind {
    CreateTable,
    Select,
    Insert,
};

struct AstNode {
    NodeKind kind;
    std::optional<size_t> value;
    std::vector<size_t> children;
};

using Ast = std::vector<AstNode>;


struct AstPush {
    Ast ast;

    size_t push_ast(
        std::optional<size_t> index,
        NodeKind kind
    ) {
        size_t id = ast.size();

        this->ast.push_back(
            AstNode{
                .kind = kind,
                .value = index,
                .children = {}
            }
        );

        return id;
    }

    void add_child_id(
        size_t parent_id, 
        size_t child_id
    ) {
        this->ast[parent_id]
            .children
            .push_back(child_id);
    }

    Ast get_ast() noexcept {
        return std::move(this->ast);
    }
};

template<ExprKind K>
struct ParenStatus : AstPush {
    enum class Flags {
        NextString,
        NextType,
        NextComma,
    } flags;
    std::vector<size_t> stack;
    size_t parent_id;

    ParenStatus() {
        parent_id = 0;
        flags = Flags::NextString;
    }

    inline std::optional<Errors> column_type(size_t count) {
        if constexpr (K == ExprKind::CreateTable) {
            switch (flags) {
                case Flags::NextType: {
                    flags = Flags::NextComma;
                    stack.push_back(count);
                    break;
                }
                case Flags::NextComma: return Errors::MissingToken;
                default: return Errors::UnexpectedToken;
            }
        } else // ほかの式で、カラムの型を処理することは無い
            return Errors::UnexpectedToken;
        return std::nullopt;
    }

    inline std::optional<Errors> string_token(size_t count) {
        if (flags == Flags::NextString) {
            stack.push_back(count);
            if constexpr (K == ExprKind::CreateTable) {
                flags = Flags::NextType;
            } else {
                flags = Flags::NextComma;
            }
        } else
            return Errors::UnexpectedToken;
        return std::nullopt;
    }

    inline std::optional<Errors> comma_token(size_t count) {
        if (flags == Flags::NextComma) {
            flags == Flags::NextString;
            if constexpr (K == ExprKind::CreateTable) {
                // カラムのヘッダを作成
                size_t id = push_ast(std::nullopt, NodeKind::Column);
                add_child_id(parent_id, id);
                // テーブルを作成する式の場合カラムの種類があるテーブルを作成する。
                constexpr NodeKind kind_table[2] =
                    {NodeKind::ColumnName, NodeKind::ColumnType};

                if (stack.empty() || stack.size() < 2) // stackが使えるか確認
                    return Errors::MissingToken;

                for (size_t i = 0; i < stack.size(); i++) {
                    add_child_id(id, push_ast(stack[i], kind_table[i]));
                }
            } else {
                push_ast(stack[0], NodeKind::ColumnName);
            }
            stack.clear();
        } else
            return Errors::MissingToken;
        return std::nullopt;
    }
};

template<ExprKind K>
std::expected<Ast, Errors> generate_paren_node(
    const std::vector<Token>& tokens,
    size_t* count_ptr
) {
    ParenStatus<K> status;
    size_t count = *count_ptr;

    while (count < tokens.size()) {
        switch (tokens[count].token_kind) {
            case TokenKind::Comma: {
                if (auto result = status.comma_token(count))
                    return std::unexpected(*result);
                break;
            }
            case TokenKind::STRING: {
                if (auto result = status.string_token(count))
                    return std::unexpected(*result);
                break;
            }
            case TokenKind::ColumnType: {
                if (auto result = status.column_type(count))
                    return std::unexpected(*result);
                break;
            }
            case TokenKind::RightParen: {
                *count_ptr = ++count;
                return status.get_ast();
            }
            default: return std::unexpected(Errors::UnexpectedToken);
        }
        count++;
    }
    return std::unexpected(Errors::SyntaxErr);
}

// exprs/create.cpp
std::expected<Ast, Errors> create_expr_node(const std::vector<Token>& tokens);
// exprs/select.cpp
std::expected<Ast, Errors> select_expr_node(std::vector<Token> tokens);

// ast.cpp
std::expected<Ast, Errors> ganarate_ast(const std::vector<Token>& tokens);