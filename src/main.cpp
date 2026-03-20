#include "analy.hpp"

int main(int argc, char *argv[]) {
    if (argc > 1) {
        std::vector<Token> tokens = input_sql_query(argv[1]);
        for (auto value: tokens) {
            std::cout << value.value << std::endl;
        }
    }

    return 0;
}