#include "parser.hpp"
#include "ast_node.hpp"


std::expected<std::vector<Ast>, Errors> Parser::create_CREATE_TABLE_node(
    const std::vector<Token>& tokens,
    size_t* pos
) {
    Parser::Match match(tokens);

    if (!match.consume(TokenKind::CREATE, pos))
        return std::unexpected(Errors::UnexpectTokenKind);
    auto result = match.expect(TokenKind::STRING, AstNodeKind::TableName, pos);
    if (result.has_value()) {
        //
    } else {
        return std::unexpected(Errors::UnexpectTokenKind);
    }
    if (!match.consume(TokenKind::LeftParen, pos))
        //
    return std::unexpected(Errors::SyntaxErr);
}