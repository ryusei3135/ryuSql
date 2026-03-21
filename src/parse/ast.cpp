#include "parser.hpp"


extern "C" void create_table_node(
    const Token* tokens_data,
    size_t len,
    size_t* token_pos
);

void generate_asts(std::vector<Token>& tokens) {
    size_t token_pos = 0;

    while (token_pos < tokens.size()) {
        //
    }
}