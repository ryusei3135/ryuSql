#include "parser.hpp"


// 期待されたトークンと現在のトークンを比較
bool Parser::Match::consume(const TokenKind kind, size_t* i)  {
    if (tokens[(*i)++].token_kind == kind)
        return true;
    else
        return false;
}

// 期待されたトークンが来た場合ノードにして返す
std::optional<Ast> Parser::Match::expect(
    const TokenKind kind, 
    const AstNodeKind node_kind, 
    size_t* pos
) {
    if (consume(kind, pos))
        return Analy::AstMake::Make(*pos++, node_kind);
    else
        return std::nullopt;
}