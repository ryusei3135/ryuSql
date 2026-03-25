#include "parser.hpp"
#include "ast_node.hpp"


// astノードを作成するapi
// このnamespaceはぜんったい省略してはいけない
namespace Parser::Node {
    enum class Child {
        Left = 0,
        Right = 1
    };

    template<AstNodeKind T>
    Ast maker(
        const size_t value,
        const std::optional<std::array<size_t, 2>> child_id
    ) {
        auto id_or_zero = [&](const Child index) -> size_t {
            if (child_id.has_value()) {
                return child_id.value()[static_cast<size_t>(index)];
            }
            return 0;
        };
        return Ast {
            .value = value,
            .kind = T,
            .left = id_or_zero(Child::Left),
            .right = id_or_zero(Child::Right)
        };
    }

    template<AstNodeKind T> Ast make_null() {
        return maker<T>(0, std::nullopt);
    }
    
    inline Ast maker_target_kind(const size_t value, const AstNodeKind kind) {
        return Ast {
            .value = value,
            .kind = kind,
            .left = 0,
            .right = 0
        };
    }

    inline void make_column_node(
        AstNode& ast,
        const size_t name,
        const size_t type
    ) {
        ast.back().left = ast.size();
        const std::array child{ast.size() + 1, ast.size() + 2};
        ast.push_back(maker<AstNodeKind::Column>(0, child));
        ast.push_back(maker<AstNodeKind::ColumnName>(name, std::nullopt));
        ast.push_back(maker<AstNodeKind::ColumnType>(type, std::nullopt));
    }
};
