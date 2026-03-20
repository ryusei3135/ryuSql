#include "parser.hpp"


enum class SelectExpectToken {
    Token_FROM_,
    TokenColumnName,
    TokenTableName,
};

auto create_SELECT_node(
    std::vector<Token>& tokens, 
    size_t* i
) -> std::expected<Ast, Errors> {
    SelectExpectToken expect = SelectExpectToken::TokenColumnName;
    size_t count = *i;
    Ast ast{};

    while (count < tokens.size()) {
        switch (tokens[count].token_kind) {
            case TokenKind::STRING: {
                switch (expect) {
                    case SelectExpectToken::TokenColumnName: {
                        ast.push_back(
                            AstNode::make(count, NodeKind::ColumnName));
                        expect = SelectExpectToken::Token_FROM_;
                        break;
                    }
                    case SelectExpectToken::TokenTableName: {
                        ast.push_back(
                            AstNode::make(count, NodeKind::TableName));
                        *i = count;
                        return ast;
                    }
                    default: {
                        return std::unexpected(Errors::UnexpectTokenKind);
                    }
                }
                break;
            }
            case TokenKind::FROM: {
                if (expect == SelectExpectToken::Token_FROM_) {
                    expect = SelectExpectToken::TokenTableName;
                } else {
                    return std::unexpected(Errors::UnexpectTokenKind);
                }
                break;
            }
            case TokenKind::Semicolon: {
                return ast;
            }
        }
        count++;
    }

    return std::unexpected(Errors::SyntaxErr);
}