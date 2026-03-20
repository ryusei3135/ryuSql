#include "parser.hpp"


namespace Paren {
    enum class ExpectToken {
        ColumnName,
        ColumnType,
        Comma
    };
};

template<NodeKind K>
auto create_paren_node(
    Ast* ast,
    std::vector<Token>& tokens, 
    size_t* i
) -> std::optional<Errors> {
    Paren::ExpectToken expect = Paren::ExpectToken::ColumnName;
    size_t count = *i;
    size_t parent_id = ast->size() - 1;

    while (count < tokens.size()) {
        switch (tokens[count].token_kind) {
            case TokenKind::ColumnType: { // テーブルを作成する式以外使わない
                if constexpr (K == NodeKind::CreateTable) {
                    if (expect == Paren::ExpectToken::ColumnType) {
                        (*ast)[parent_id].children->push_back(ast->size());
                        ast->push_back(
                            AstNode::make(count, NodeKind::ColumnName));
                        expect = Paren::ExpectToken::Comma;
                        break;
                    }
                }
                return Errors::UnexpectTokenKind;
            }
            case TokenKind::Comma: {
                if (expect == Paren::ExpectToken::Comma) {
                    expect = Paren::ExpectToken::ColumnName;
                    break;
                }
                return Errors::UnexpectTokenKind;
            }
            case TokenKind::STRING: {
                if (expect == Paren::ExpectToken::ColumnName) {
                    (*ast)[parent_id].children->push_back(ast->size());
                    ast->push_back(AstNode::make(count, NodeKind::ColumnName));
                    if constexpr (K == NodeKind::CreateTable) {
                        expect = Paren::ExpectToken::ColumnType;
                    } else {
                        expect = Paren::ExpectToken::Comma;
                    }
                    break;
                }
                return Errors::UnexpectTokenKind;
            }
            case TokenKind::RightParen: {
                return std::nullopt;
            }
            default: {
                return Errors::InvalidTokenKind;
            }
        }
        count++;
    }
    return Errors::SyntaxErr;
}