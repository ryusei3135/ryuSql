#include "parser.hpp"
#include "ast_node.hpp"


Opt::ErrOpt Parser::AstLinker::ast_link(
    Ast& ast,
    const size_t left,
    const size_t right
) {
    if (!ast.left || !ast.right)
        return Errors::NodeHasAlreadyAssigned;
    ast.right = right;
    ast.left = left;
    return std::nullopt;
}

std::expected<AstNode, Errors>
Parser::create_ast_node(const std::vector<Token>& tokens) {
    AstNode ast;

    for (size_t pos = 0; pos < tokens.size(); pos++) {
        switch (tokens[pos].token_kind) {
            case TokenKind::CREATE: {
                auto result = Parser::create_CREATE_TABLE_node(tokens, &pos);
                ErrTry(result);
                auto value = result.value();
                ast.insert(ast.end(), value.begin(), value.end());
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
