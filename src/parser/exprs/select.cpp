#include "parser.hpp"


namespace flag {
    enum class SelectExprFlag {
        NextSelect,
        NextColumnName,
        NextFrom,
        NextTableName,
    };
};

struct SelectExpr : AstPush {
    flag::SelectExprFlag expr_flag;
    std::vector<uint32_t> column_name;

    SelectExpr() {
        expr_flag = flag::SelectExprFlag::NextSelect;
    }

    inline std::optional<Errors> token_select() {
        if (expr_flag == flag::SelectExprFlag::NextSelect) {
            expr_flag = flag::SelectExprFlag::NextColumnName;
        } else {
            return Errors::SyntaxErr;
        }

        return std::nullopt;
    }

    inline std::optional<Errors> token_string(uint32_t i) {
        if (expr_flag == flag::SelectExprFlag::NextColumnName) {
            expr_flag = flag::SelectExprFlag::NextFrom;
            column_name.push_back(i);
        } else if (expr_flag == flag::SelectExprFlag::NextTableName) {
            push_ast<void>(i, NodeKind::SelectTable);

            if (column_name.empty())
                return Errors::MissingColumn;

            for (uint32_t index: column_name) {
                add_child_id(
                    0,
                    push_ast<uint8_t>(
                        index,
                        NodeKind::SelectColumn
                    )
                );
            }
        } else {
            return Errors::UnexpectedToken;
        }

        return std::nullopt;
    }

    inline std::optional<Errors> token_from() {
        if (expr_flag == flag::SelectExprFlag::NextFrom) {
            expr_flag = flag::SelectExprFlag::NextTableName;
        } else {
            return Errors::UnexpectedToken;
        }

        return std::nullopt;
    }
};

std::expected<Ast, Errors> select_expr_node(std::vector<Token> tokens) {
    SelectExpr status;

    for (uint32_t i = 0; i < tokens.size(); i++) {
        switch (tokens[i].token_kind) {
            case TokenKind::SELECT: {
                if (auto result = status.token_select())
                    return std::unexpected(result.value());
                break;
            }
            case TokenKind::STRING: {
                if (auto result = status.token_string(i))
                    return std::unexpected(result.value());
                break;
            }
            case TokenKind::FROM: {
                if (auto result = status.token_from())
                    return std::unexpected(result.value());
                break;
            }
            case TokenKind::Semicolon: {
                return status.get_ast();
            }
            default: {
                return std::unexpected(Errors::UnexpectedToken);
            }
        }
    }

    return status.get_ast();
}