#pragma once

#include "parser.hpp"
#include <span>


enum class OpCode {
    CreateTable,
    AddColumn,
    ColumnType,
    EndTable,

    SelectTarget,
    SelectColumn,
    SelectEnd,
};

struct Instruction {
    OpCode op;
    size_t arg;
};

using IR = std::vector<Instruction>;


struct IrBuilder {
    IR ir;

    void push_ir(OpCode op, size_t arg) {
        ir.push_back(
            Instruction {
                .op = op,
                .arg = arg
            }
        );
    }
};

namespace ApiIR {
    std::expected<IR, Errors> create_table(Ast& ast, size_t* i);
    std::expected<IR, Errors> select_expr(Ast& ast, size_t* i);
};

void build_ir(Ast& ast);