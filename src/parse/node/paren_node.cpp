#include "parser.hpp"


std::optional<std::vector<Ast>> Parser::create_paren_node(
    const std::vector<Token>& tokens, 
    size_t* pos
) {
    Parser::Match match(tokens);
    std::vector<Ast> ast;

    for (size_t i = *pos; i < tokens.size(); i++) {
        auto r = match.expect(TokenKind::STRING, AstNodeKind::ColumnName, &i);

        if (r.has_value()) // 最初は名前
            ast.push_back(r.value());
        if (auto r = match.expect(TokenKind::ColumnType, AstNodeKind::ColumnType, &i))
            ast.push_back(r.value());
        if (match.consume(TokenKind::Comma, &i))
            continue;

        if (match.consume(TokenKind::RightParen, &i)) {
            *pos = i;
            return ast;
        }

        return std::nullopt;
    }

    return ast;
}