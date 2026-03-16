#include "parser.hpp"
#include "analy.hpp"


namespace flag {
    //  式のASTを作成中に次のトークンを予約する
    enum class ExprFlags {
        NextCreate,
        NextTable,
        NextString,
        NextLeftParen,
        NextColumn,
        NextRightParen,
    };

    // カラムのノードを作成する際に、次に予約するトークンの種類
    enum class ColumnFlags {
        NextName,
        NextType,
    };
};

struct CreateTable : AstPush {
    flag::ExprFlags expr_flags;
    flag::ColumnFlags column_flags;
    std::array<std::optional<size_t>, 2> column_values{};
    std::optional<size_t> table_name_index;

    CreateTable() {
        expr_flags = flag::ExprFlags::NextCreate;
        column_flags = flag::ColumnFlags::NextName;
        table_name_index = std::nullopt;

        push_ast<void>(
            std::nullopt,
            NodeKind::Table
        );
    }

    inline std::optional<Errors> string_token(size_t i) {
        if (expr_flags == flag::ExprFlags::NextColumn) {
            if (column_flags == flag::ColumnFlags::NextName) {
                column_flags = flag::ColumnFlags::NextType;
                column_values[0] = std::make_optional(i);
            } else {
                return Errors::SyntaxErr;
            }
        } else if (expr_flags == flag::ExprFlags::NextString) {
            if (!table_name_index) {
                expr_flags = flag::ExprFlags::NextLeftParen;
            } else {
                std::cout << "expr is false" << std::endl;
            }
            table_name_index = i;
        } else {
            return Errors::UnexpectedToken;
        }

        return std::nullopt;
    }

    inline std::optional<Errors> left_paren_token() {
        if (expr_flags == flag::ExprFlags::NextLeftParen) {
            expr_flags = flag::ExprFlags::NextColumn;
            // テーブルの名前があるか確認
            if (table_name_index) {
                push_ast<void>(
                    *table_name_index,
                    NodeKind::TableName
                );
            } else {
                return Errors::MissingTableName;
            }
        } else {
            return Errors::SyntaxErr;
        }
        return std::nullopt;
    }

    inline std::optional<Errors> generate_column_node() {
        constexpr NodeKind column_table[2] = {
            NodeKind::ColumnName,
            NodeKind::ColumnType};

        size_t parent_id = ast.size();
        add_child_id(
            0,
            push_ast<uint8_t>(
                std::nullopt,
                NodeKind::Column
            )
        );

        for (size_t i = 0; i < column_values.size();i++) {
            if (!column_values[i])
                break;

            add_child_id(
                parent_id,
                push_ast<uint8_t>(
                    *column_values[i],
                    column_table[i]
                )
            );
        }
        // カラムのフラグを初期化
        std::fill(column_values.begin(), column_values.end(), std::nullopt);
        column_flags = flag::ColumnFlags::NextName;
        expr_flags = flag::ExprFlags::NextColumn;

        return std::nullopt;
    }

    inline Errors search_TABLE_err() {
        if (expr_flags == flag::ExprFlags::NextCreate) {
            return Errors::Missing_CREATE_keyword;
        } else {
            return Errors::SyntaxErr;
        }
    }
};

std::expected<Ast, Errors> create_expr_node(const std::vector<Token>& tokens) {
    CreateTable status;

    for (size_t i = 0; i < tokens.size(); i++) {
        switch (tokens[i].token_kind) {
            case TokenKind::CREATE: {
                if (status.expr_flags == flag::ExprFlags::NextCreate) {
                    if (!status.table_name_index) {
                        status.expr_flags = flag::ExprFlags::NextTable;
                    } else {
                        std::cout << "expr is false" << std::endl;
                    }
                } else {
                    return std::unexpected(Errors::UnexpectedToken);
                }
                break;
            }
            case TokenKind::TABLE: {
                if (status.expr_flags == flag::ExprFlags::NextTable) {
                    if (!status.table_name_index) {
                        status.expr_flags = flag::ExprFlags::NextString;
                    } else {
                        return std::unexpected(Errors::SyntaxErr);
                    }
                } else {
                    return std::unexpected(status.search_TABLE_err());
                }
                break;
            }
            case TokenKind::STRING: {
                if (auto result = status.string_token(i))
                    return std::unexpected(result.value());
                break;
            }
            case TokenKind::LeftParen: {
                if (auto result = status.left_paren_token())
                    return std::unexpected(result.value());
                break;
            }
            case TokenKind::RightParen: {
                // テーブルの定義が終了
                if (status.expr_flags == flag::ExprFlags::NextColumn) {
                    if (status.column_values[0] && status.column_values[1])
                        if (auto result = status.generate_column_node())
                            return std::unexpected(result.value());
                    return status.ast;
                } else {
                    return std::unexpected(Errors::Missing_left_paren_token);
                }
                break;
            }
            case TokenKind::ColumnType: {
                if (status.column_flags == flag::ColumnFlags::NextType) {
                    status.column_values[1] = i;
                } else {
                    return std::unexpected(Errors::SyntaxErr);
                }
                break;
            }
            case TokenKind::Comma: {
                if (auto result = status.generate_column_node())
                    return std::unexpected(result.value());
                break;
            }
            default: {
                return std::unexpected(Errors::UnexpectedToken);
            }
        }
    }

    return status.get_ast();
}