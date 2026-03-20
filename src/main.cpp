#include "analy.hpp"
#include "parser.hpp"


int main(int argc, char *argv[]) {
    if (argc > 1) {
        Ast ast;

        auto tokens = input_sql_query(argv[1]);
        if (!tokens.has_value()) {
            std::cout << (int)tokens.error() << ":[err]token" << std::endl;
            return 1;
        }

        auto err = create_ast(&ast, tokens.value());
        if (err.has_value()) {
            std::cout << (int)err.value() << ":[err]ast" << std::endl;
            return 1;
        }

        for (auto n: ast) {
            std::cout << n.value << std::endl;
        }
    }

    return 0;
}