#include "parser.hpp"
#include "RyuSql.hpp"


namespace Parser {
    // 現在のトークンと渡されたトークンの種類を比較し作成したノードや
    // optionalを返す
    class NodeEmitter {
    public:
        NodeEmitter(
            const std::vector<Token>& ref_tokens
        ) : tokens(ref_tokens) {}

        // トークンの種類を比較
        template<TokenKind K>
        inline Opt::ErrOpt compare_kind(size_t* i) const {
            if (tokens[*i].token_kind == K) {
                (*i)++;
                return std::nullopt;
            }
            return Errors::UnexpectTokenKind;
        }

        // トークンの種類を比較しノードを作成して返す
        // ノードを返す際にすでに"compare_kind"でインクリメントしているので
        // この関数では"i"には何もしない
        template<TokenKind K>
        inline std::expected<Ast, Errors>
        expect_kind(const AstNodeKind kind, size_t* i) const {
            if (!compare_kind<K>(i))
                return AstNodeMaker::Make(*i, kind);
            return std::unexpected(Errors::UnexpectTokenKind);
        }
    private:
        std::vector<Token> tokens;
    };
};