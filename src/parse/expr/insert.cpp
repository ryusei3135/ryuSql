#include "parser.hpp"
#include "paren_node.hpp"


enum class ExpectTokenKind {
    INTO,
    TableName,
    Columns,
    VALUES,
    Values
};

std::expected<Ast, Errors> create_INSERT_node(
    std::vector<Token> tokens, 
    size_t* i
) {
    ExpectTokenKind expect = ExpectTokenKind::INTO;
    size_t count = *i;
    Ast ast;

    while (count < tokens.size()) {
        switch (tokens[count].token_kind) {
            case TokenKind::INTO: {
                if (expect == ExpectTokenKind::INTO) {
                    expect = ExpectTokenKind::Columns;
                } else {
                    return std::unexpected(Errors::UnexpectTokenKind);
                }
                break;
            }
            case TokenKind::LeftParen: {
                if (expect == ExpectTokenKind::Columns) {
                    if (auto err = create_paren_node<NodeKind::InsertColumns>
                        (&ast, tokens, &++count)
                    ) {
                        return std::unexpected(err.value());
                    }
                    expect = ExpectTokenKind::VALUES;
                } else if (expect == ExpectTokenKind::Values) {
                    if (auto err = create_paren_node<NodeKind::InsertValues>
                        (&ast, tokens, &++count)
                    ) {
                        return std::unexpected(err.value());
                    }
                    *i = count;
                    return ast;
                } else {
                    return std::unexpected(Errors::UnexpectTokenKind);
                }
                break;
            }
            case TokenKind::VALUES: {
                if (expect == ExpectTokenKind::VALUES) {
                    expect = ExpectTokenKind::Values;
                } else {
                    return std::unexpected(Errors::UnexpectTokenKind);
                }
                break;
            }
            case TokenKind::STRING: {
                if (expect == ExpectTokenKind::TableName) {
                    ast.push_back(AstNode::make(count, NodeKind::TableName));
                    expect = ExpectTokenKind::Columns;
                } else {
                    return std::unexpected(Errors::UnexpectTokenKind);
                }
                break;
            }
            default:
                return std::unexpected(Errors::UnexpectTokenKind);
        }
        count++;
    }
    return std::unexpected(Errors::SyntaxErr);
}