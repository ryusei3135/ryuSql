#include "parser.hpp"

namespace Node {
    template<AstNodeKind K>
    std::expected<std::vector<Ast>, Errors>
    create_paren_node(const std::vector<Token>& tokens, size_t* pos) {
        for (size_t i = *pos; i < tokens.size(); i++) {
            //
        }
        return std::unexpected(Errors::SyntaxErr);
    }
};