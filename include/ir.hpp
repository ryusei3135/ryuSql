#pragma once

#include "parser.hpp"
#include <span>


enum class OpCode {
    // table
    CreateTable,
    AddColumn,
    ColumnType,
    EndTable,
    // select
    SelectTarget,
    SelectColumn,
    SelectEnd,
    // insert
    InsertTarget,
    InsertColumn,
    InsertValues,
    InsertEnd,
};

struct Instruction {
    OpCode op;
    std::optional<size_t> arg;
};

using IR = std::vector<Instruction>;


struct IrBuilder {
    IR ir;

    void push_ir(OpCode op, std::optional<size_t> arg) {
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

std::expected<IR, Errors> build_ir(Ast& ast);