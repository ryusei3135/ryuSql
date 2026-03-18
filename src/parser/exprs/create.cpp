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
};

struct CreateTable : AstPush {
    flag::ExprFlags expr_flags;
    std::array<std::optional<size_t>, 2> column_values{};
    std::optional<size_t> table_name_index;
    std::optional<size_t> parent_id;

    CreateTable() {
        expr_flags = flag::ExprFlags::NextCreate;
        table_name_index = std::nullopt;
        parent_id = std::nullopt;
    }

    inline std::optional<Errors> string_token(size_t i) {
        if (expr_flags == flag::ExprFlags::NextString) {
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

    inline Errors search_TABLE_err() {
        if (expr_flags == flag::ExprFlags::NextCreate) {
            return Errors::MissingToken;
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
                if (status.expr_flags == flag::ExprFlags::NextLeftParen) {
                    auto result = generate_paren_node<ExprKind::CreateTable>
                        (tokens, &++i);
                    if (result.has_value()) {
                        auto value = result.value();
                        status.ast.insert(
                            status.ast.end(), 
                            value.begin(), 
                            value.end()
                        );
                    } else {
                        return result;
                    }
                }
                break;
            }
            default: {
                return std::unexpected(Errors::UnexpectedToken);
            }
        }
    }

    return std::unexpected(Errors::MissingToken);
}