#include <fstream>
#include <filesystem>

#include "analy.hpp"
#include "parser.hpp"
#include "ir.hpp"
#include "exec.hpp"


void write_head_info(char* file_name) {
    std::ofstream ofs(file_name, std::ios::binary);
    uint8_t head_info[8] = {
        'R','Y','U','S','Q','L',0xFF,0xFF
    };
    ofs.write(reinterpret_cast<char*>(&head_info), sizeof(head_info));
}

std::optional<std::string> get_sql() {
    std::string line;
    std::string query;
    std::cout << "input:" << std::endl;

    while (std::getline(std::cin, line)) {
        if (line == "%push")
            break;
        if (line == "%exit")
            return std::nullopt;
        query.append(line);
    }

    return query;
}

int main(int argc, char *argv[]) {
    if (argc > 1) {
        bool exists = std::filesystem::exists(argv[1]);

        // if (exists) {
        //     std::ifstream ifs(argv[1], std::ios::binary);
        //     int x;
        //     ifs.read(reinterpret_cast<char*>(&x), sizeof(x));
        // } else {
        //     write_head_info(argv[1]);
        // }

        while (true) {
            std::optional<std::string> query = get_sql();
            if (!query) {
                break;
            }
            std::vector<Token> tokens = input_sql_query(query.value().c_str());
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