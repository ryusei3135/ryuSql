#include "ir.hpp"


void build_ir(Ast& ast) {
    for (size_t i = 0; i < ast.size(); i++) {
        switch (ast[i].kind) {
            case NodeKind::Table: {
                auto result = ApiIR::create_table(ast, &++i);
                std::cout << result.has_value() << std::endl;
            }
            case NodeKind::Select: {}
        }
    }
}