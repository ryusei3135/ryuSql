#include "parser.hpp"
#include "analy.hpp"


std::expected<Ast, Errors> ganarate_ast(const std::vector<Token>& tokens) {
    std::expected<Ast, Errors> result;

    for (size_t i = 0; i < tokens.size(); i++) {
        std::cout << tokens[i].value << std::endl;
        switch (tokens[i].token_kind) {
            case TokenKind::CREATE: {
                result = create_expr_node(tokens);
                break;
            }
            case TokenKind::SELECT: {
                result = select_expr_node(tokens);
                break;
            }
            default: {
                break;
            }
        }
    }
    return result;
}