#include "parser.hpp"
#include "ast_node.hpp"


std::expected<std::vector<Ast>, Errors> Parser::create_CREATE_TABLE_node(
    const std::vector<Token>& tokens,
    size_t* pos
) {
    Parser::NodeEmitter emitter(tokens);
    std::vector<Ast> ast;
    // テーブルの名前が来ない場合例外を返すので、optionalにする必要はない
    size_t parent_id;

    ErrTry(emitter.compare_kind<TokenKind::CREATE>(pos));
    ErrTry(emitter.compare_kind<TokenKind::TABLE>(pos));

    if (auto result = emitter.expect_kind<TokenKind::STRING>(
        AstNodeKind::OpCreateTable, pos
    )) {
        parent_id = ast.size();
        ast.push_back(result.value());
    } else {
        #ifdef DEBUG
        std::cerr << "[node:err]: unmatch token (table name)" << std::endl;
        #endif
        return std::unexpected(Errors::UnexpectTokenKind);
    }

    ErrTry(emitter.compare_kind<TokenKind::LeftParen>(pos));
    if (auto result = Parser::create_paren_node(tokens, pos, parent_id)) {
        std::vector<Ast> paren_node = result.value();
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