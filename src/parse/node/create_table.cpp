#include "parser.hpp"
#include "ast_node.hpp"


std::expected<std::vector<Ast>, Errors> Parser::create_CREATE_TABLE_node(
    const std::vector<Token>& tokens,
    size_t* pos
) {
    Parser::Match match(tokens);
    std::vector<Ast> ast;

    if (!match.consume(TokenKind::CREATE, pos)) {
        #ifdef DEBUG
        std::cerr << "[node:err]: unmatch token (CREATE)" << std::endl;
        #endif
        return std::unexpected(Errors::UnexpectTokenKind);
    }

    std::cout << tokens[*pos].value << std::endl;
    if (!match.consume(TokenKind::TABLE, pos)) {
        #ifdef DEBUG
        std::cerr << "[node:err]: unmatch token (TABLE)" << tokens[*pos].value << std::endl;
        #endif
        return std::unexpected(Errors::UnexpectTokenKind);
    }

    if (auto result = match.expect(
        TokenKind::STRING, AstNodeKind::OpCreateTable, pos
    )) {
        ast.push_back(result.value());
    } else {
        #ifdef DEBUG
        std::cerr << "[node:err]: unmatch token (table name)" << std::endl;
        #endif
        return std::unexpected(Errors::UnexpectTokenKind);
    }

    if (match.consume(TokenKind::LeftParen, pos)) {
        if (auto result = Parser::create_paren_node(tokens, pos)) {
            std::vector<Ast> value = result.value();
            ast.insert(ast.end(), value.begin(), value.end());
            return ast;
        } else {
            #ifdef DEBUG
            std::cerr << "[node:err]: paren node" << std::endl;
            #endif
            return std::unexpected(Errors::UnexpectTokenKind);
        }
    }

    return std::unexpected(Errors::SyntaxErr);
}