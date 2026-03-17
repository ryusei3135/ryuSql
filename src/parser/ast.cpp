#include "parser.hpp"
#include "analy.hpp"


std::expected<Ast, Errors> ganarate_ast(const std::vector<Token>& tokens) {
    std::expected<Ast, Errors> result;

    switch (tokens[0].token_kind) {
        case TokenKind::CREATE: {
            result = create_expr_node(tokens);
            break;
        }
        case TokenKind::SELECT: {
            result = select_expr_node(tokens);
            break;
        }
    }
    return result;
}