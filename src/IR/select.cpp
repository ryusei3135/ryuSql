#include "ir.hpp"


std::expected<IR, Errors> ApiIR::select_expr(Ast& ast, size_t* i) {
    IrBuilder builder;

    for (size_t count = *i; count < ast.size(); count++) {
        switch (ast[count].kind) {
            case NodeKind::SelectTable: {
                builder.push_ir(OpCode::SelectTarget, count);
                break;
            }
            case NodeKind::SelectColumn: {
                builder.push_ir(OpCode::SelectColumn, count);
                break;
            }
            default: {
                std::cout << "select" << count << std::endl;
                break;
            }
        }

        *i = count;
    }

    return std::move(builder.ir);
}