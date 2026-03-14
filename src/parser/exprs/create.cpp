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
        this->expr_flags = flag::ExprFlags::NextCreate;
        this->column_flags = flag::ColumnFlags::NextName;
        this->table_name_index = std::nullopt;
    }

    inline std::optional<Errors> string_token(size_t i) {
        if (this->expr_flags == flag::ExprFlags::NextColumn) {
            if (this->column_flags == flag::ColumnFlags::NextName) {
                this->column_flags = flag::ColumnFlags::NextType;
                this->column_values[0] = std::make_optional(i);
            } else {
                return Errors::SyntaxErr;
            }
        } else if (this->expr_flags == flag::ExprFlags::NextString) {
            if (!this->table_name_index) {
                this->expr_flags = flag::ExprFlags::NextLeftParen;
            } else {
                std::cout << "expr is false" << std::endl;
            }
            this->table_name_index = i;
        } else {
            return Errors::UnexpectedToken;
        }

        return std::nullopt;
    }

    inline std::optional<Errors> left_paren_token() {
        if (this->expr_flags == flag::ExprFlags::NextLeftParen) {
            this->expr_flags = flag::ExprFlags::NextColumn;
            // テーブルの名前があるか確認
            if (this->table_name_index) {
                this->push_ast<void>(
                    *this->table_name_index,
                    NodeKind::Table
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
        
        for (size_t i = 0; i < column_values.size();i++) {
            if (!column_values[i])
                break;

            this->add_child_id(
                0,
                this->push_ast<uint8_t>(
                    *column_values[i],
                    column_table[i]
                )
            );
        }
        // カラムのフラグを初期化
        std::fill(this->column_values.begin(), this->column_values.end(), std::nullopt);
        this->column_flags = flag::ColumnFlags::NextName;
        this->expr_flags = flag::ExprFlags::NextColumn;

        return std::nullopt;
    }

    inline Errors search_TABLE_err() {
        if (this->expr_flags == flag::ExprFlags::NextCreate) {
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