#include "analy.hpp"
#include "parser.hpp"

// # ===== RyuSql.hpp ===== #
Ast AstNodeMaker::Make(const size_t value, const AstNodeKind K) {
    return Ast {
        .value = value,
        .kind = K,
        .left = 0,
        .right = 0
    };
}

// # ===== main.cpp ===== #
int main(int argc, char *argv[]) {
    if (argc > 1) {
        auto tokens = Analy::input_sql_query(argv[1]);
        if (!tokens.has_value()) {
            std::cout << (int)tokens.error() << ":[err]token" << std::endl;
            return 1;
        }

        auto result = Parser::create_ast_node(tokens.value());
        if (!result.has_value()) {
            std::cout << (int)result.error() << ":[err]ast" << std::endl;
            return 1;
        }

        for (auto n: result.value()) {
            std::cout << n.value << std::endl;
        }
    }

    return 0;
}