#include "parser.hpp"


std::expected<Ast, Errors> create_ast(std::vector<Token>& tokens) {
    Ast ast;
    auto connect_node = [&](auto func, size_t* i) -> std::optional<Errors> {
        ast.push_back(AstNode::make(0, NodeKind::Start));
        auto result = func(tokens, i);
        if (!result.has_value()) {
            return result.error();
        }
        auto add_node = result.value();
        ast.insert(ast.end(), add_node.begin(), add_node.end());
        ast.push_back(AstNode::make(0, NodeKind::End));
        return std::nullopt;
    };

    for (size_t i = 0; i < tokens.size(); i++) {
        switch (tokens[i].token_kind) {
            case TokenKind::CREATE: {
                if (auto err = connect_node(create_CREATE_TABLE_node, &++i))
                    return std::unexpected(err.value());
                break;
            }
            case TokenKind::SELECT: {
                if (auto err = connect_node(create_SELECT_node, &++i))
                    return std::unexpected(err.value());
                break;
            }
            default:
                break;
        }
    }

    return ast;
}