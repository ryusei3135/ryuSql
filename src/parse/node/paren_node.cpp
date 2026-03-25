#include "parser.hpp"

// パーレンのトークンを処理する
std::expected<AstNode, Errors> Parser::create_paren_node(
    const std::vector<Token>& tokens,
    size_t* pos
) {
    Parser::TokenKindMatcher emitter(tokens);
    AstNode ast;
    ast.push_back(Parser::Node::make_null<AstNodeKind::ColumnNext>());

    for (size_t i = *pos; i < tokens.size(); i++) {
        size_t name = i;
        ErrTry(emitter.compare_kind<TokenKind::STRING>(&i));
        size_t type = i;
        ErrTry(emitter.compare_kind<TokenKind::ColumnType>(&i));

        Parser::Node::make_column_node(ast, name, type);

        if (!emitter.compare_kind<TokenKind::Comma>(&i)) {
            continue;
        }

        ErrTry(emitter.compare_kind<TokenKind::RightParen>(&i));
        *pos = i;
        return ast;
    }

    return std::unexpected(Errors::MissingTokenKind);
}
