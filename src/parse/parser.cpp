#include "parser.hpp"


bool Parser::Match::consume(const TokenKind kind, size_t* i)  {
    if (tokens[*i].token_kind == kind) {
        (*i)++;
        return true;
    }
    return false;
}

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