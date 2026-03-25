#include "parser.hpp"
#include "ast_node.hpp"


std::expected<AstNode, Errors> Parser::create_CREATE_TABLE_node(
    const std::vector<Token>& tokens,
    size_t* pos
) {
    Parser::TokenKindMatcher emitter(tokens);
    AstNode ast;
    ast.push_back(Parser::Node::make_null<AstNodeKind::Header>());

    ErrTry(emitter.compare_kind<TokenKind::CREATE>(pos));
    ErrTry(emitter.compare_kind<TokenKind::TABLE>(pos));
    ast.push_back(Parser::Node::make_null<AstNodeKind::OpCreateTable>());

    if (auto result = emitter.expect_kind<TokenKind::STRING>(
        AstNodeKind::TableName, pos
    )) {
        ast.push_back(result.value());
    } else {
        #ifdef DEBUG
        std::cerr << "[node:err]: unmatch token (table name)" << std::endl;
        #endif
        return std::unexpected(Errors::UnexpectTokenKind);
    }

    ErrTry(emitter.compare_kind<TokenKind::LeftParen>(pos));
    if (auto result = Parser::create_paren_node(tokens, pos)) {
        ast[1].right = ast.size();
        AstNode paren_node = result.value();
        ast.insert(ast.end(), paren_node.begin(), paren_node.end());
        return ast;
    } else {
        #ifdef DEBUG
        std::cerr << "[node:err]: paren node" << (int)result.error() << std::endl;
        #endif
        return std::unexpected(Errors::UnexpectTokenKind);
    }

    return std::unexpected(Errors::SyntaxErr);
}
