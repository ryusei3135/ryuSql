#include "analy.hpp"
#include "parser.hpp"
#include "ir.hpp"
#include "exec.hpp"


int main(int argc, char *argv[]) {
    if (argc > 1) {
        std::vector<Token> tokens = input_sql_query(argv[1]);
        std::expected<Ast, Errors> ast = ganarate_ast(tokens);
        
        if (ast.has_value()) {
            std::expected<IR, Errors> ir = build_ir(ast.value());
            if (ir.has_value()) {
                executor(ir.value(), tokens);
            }
        } else {
            output_err_log(ast.error());
        }
    }

    return 0;
}