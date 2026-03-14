#include "analy.hpp"
#include "parser.hpp"


int main(int argc, char *argv[]) {
    if (argc > 1) {
        std::vector<Token> tokens = input_sql_query(argv[1]);
        std::expected<Ast, Errors> ast = ganarate_ast(tokens);
        
        if (ast.has_value()) {
            for (AstNode node: ast.value()) {
                std::cout << tokens[node.value].value << std::endl;
            }
        } else {
            output_err_log(ast.error());
        }
    }

    return 0;
}