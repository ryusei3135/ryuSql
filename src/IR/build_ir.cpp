#include "ir.hpp"
#include "parser.hpp"

// 戻り値のIRと今あるIRを結合
std::optional<Errors> connect_ir(IR* ir, auto result) {
    if (result.has_value()) {
        auto value = result.value();
        ir->insert(ir->end(), value.begin(), value.end());
    } else {
        return result.error();
    }

    return std::nullopt;
}

// IRを作成
std::expected<IR, Errors> build_ir(Ast& ast) {
    IR ir;

    for (size_t i = 0; i < ast.size(); i++) {
        switch (ast[i].kind) {
            case NodeKind::Table: {
                if (auto r = connect_ir(&ir, ApiIR::create_table(ast, &++i)))
                    return std::unexpected(r.value());
                break;
            }
            case NodeKind::Select: {
                connect_ir(&ir, ApiIR::select_expr(ast, &++i));
                break;
            }
            default: {
                return std::unexpected(Errors::UnexpectedNode);
            }
        }
    }

    return ir;
}