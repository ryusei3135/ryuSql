#include "parser.hpp"


std::optional<Errors> create_ast(Ast* ast, std::vector<Token>& tokens) {
    for (size_t i = 0; i < tokens.size(); i++) {
        switch (tokens[i].token_kind) {
            case TokenKind::CREATE: {
                ast->push_back(AstNode::make(0, NodeKind::Start));
                auto result = create_CREATE_TABLE_node(tokens, &++i);
                if (!result.has_value()) {
                    return result.error();
                }
                auto add_node = result.value();
                ast->insert(ast->end(), add_node.begin(), add_node.end());
                ast->push_back(AstNode::make(0, NodeKind::End));
                break;
            }
            case TokenKind::SELECT:
                break;
            default:
                break;
        }
    }

    return std::nullopt;
}