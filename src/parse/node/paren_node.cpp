#include "parser.hpp"

// パーレンのトークンを処理する
std::expected<std::vector<Ast>, Errors> Parser::create_paren_node(
    const std::vector<Token>& tokens, 
    size_t* pos,
    const size_t parent_id
) {
    Parser::NodeEmitter emitter(tokens);
    std::vector<Ast> ast;
    size_t name_id = 0;
    size_t type_id = 0;

    for (size_t i = *pos; i < tokens.size(); i++) {
        auto r = emitter.expect_kind<TokenKind::STRING>(AstNodeKind::ColumnName, &i);

        if (r.has_value()) // 最初は名前]
            name_id = parent_id + 1; // カラムの名前のidはテーブルの名前の次
            ast.push_back(r.value());
        if (auto r = emitter.expect_kind<TokenKind::ColumnType>(
            AstNodeKind::ColumnType, 
        &i)) {
            //
            type_id = parent_id + 2;
            ast.push_back(r.value());
        }
        if (!emitter.compare_kind<TokenKind::Comma>(&i)) {
            continue;
        }

        ErrTry(emitter.compare_kind<TokenKind::RightParen>(&i));
        *pos = i;
        return ast;
    }

    return std::unexpected(Errors::MissingTokenKind);
}