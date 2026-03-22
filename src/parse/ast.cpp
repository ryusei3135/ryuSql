#include "parser.hpp"
#include "ast_node.hpp"


std::expected<std::vector<Ast>, Errors>
Parser::create_ast_node(const std::vector<Token>& tokens) {
    std::vector<Ast> ast;

    for (size_t pos = 0; pos < tokens.size(); pos++) {
        switch (tokens[pos].token_kind) {
            case TokenKind::CREATE: {
                Parser::create_CREATE_TABLE_node(tokens, &pos);
                break;
            }
            case TokenKind::INSERT: {
                break;
            }
            case TokenKind::SELECT: {
                break;
            }
            default: {
                return std::unexpected(Errors::UnexpectTokenKind);
            }
        }
    }
    return ast;
}