#include "parser.hpp"
#include "paren_node.hpp"


enum class ExpectToken {
    Token_TABLE_,
    TokenTableName,
    TokenLeftParen,
};

auto create_CREATE_TABLE_node(
    std::vector<Token>& tokens, 
    size_t* i
) -> std::expected<Ast, Errors> {
    ExpectToken expect = ExpectToken::Token_TABLE_;
    Ast ast;
    size_t count = *i;

    while (count < tokens.size()) {
        switch (tokens[count].token_kind) {
            case TokenKind::TABLE:
                if (expect == ExpectToken::Token_TABLE_) {
                    expect = ExpectToken::TokenTableName;
                    break;
                } else {
                    return std::unexpected(Errors::UnexpectTokenKind);
                }
            case TokenKind::STRING:
                if (expect == ExpectToken::TokenTableName) {
                    ast.push_back(AstNode::make(count, NodeKind::CreateTable));
                    expect = ExpectToken::TokenLeftParen;
                    break;
                } else {
                    return std::unexpected(Errors::UnexpectTokenKind);
                }
            case TokenKind::LeftParen:
                if (expect == ExpectToken::TokenLeftParen) {
                    if (auto err = create_paren_node<NodeKind::CreateTable>
                        (&ast, tokens, &++count)
                    ) {
                        return std::unexpected(err.value());
                    }
                    return ast;
                } else {
                    return std::unexpected(Errors::UnexpectTokenKind);
                }
            default:
                return std::unexpected(Errors::UnexpectTokenKind);
        }
        count++;
    }

    *i = count;
    return ast;
}