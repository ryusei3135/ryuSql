#include <fstream>
#include <filesystem>

#include "analy.hpp"
#include "parser.hpp"
#include "ir.hpp"
#include "exec.hpp"


// std::ofstream ofs("data.bin", std::ios::binary);

//     int x = 123;

//     ofs.write(reinterpret_cast<char*>(&x), sizeof(x));
// std::ifstream ifs("data.bin", std::ios::binary);

//     int x;

//     ifs.read(reinterpret_cast<char*>(&x), sizeof(x));

std::optional<char*> get_sql() {
    std::string line;
    std::string query;
    std::cout << "input:\n" << std::endl;

    while (std::getline(std::cin, line)) {
        if (line == "%push")
            break;
        if (line == "%exit")
            return std::nullopt;
        query += line;
    }

    return query.data();
}

int main(int argc, char *argv[]) {
    if (argc > 1) {
        bool exists = std::filesystem::exists(argv[1]);
        while (true) {
            auto query = get_sql();
            if (!query) {
                break;
            }
            std::vector<Token> tokens = input_sql_query(query.value());
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
    }

    return 0;
}