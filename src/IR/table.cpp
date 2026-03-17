#include "ir.hpp"


std::expected<IR, Errors> ApiIR::create_table(Ast& ast, size_t* i) {
    IrBuilder builder;
    std::optional<size_t> column_name = std::nullopt;

    for (size_t count = *i; count < ast.size(); count++) {
        switch (ast[count].kind) {
            case NodeKind::TableName: {
                builder.push_ir(OpCode::CreateTable, count);
                break;
            }
            case NodeKind::Column: {
                if (ast[++count].kind == NodeKind::ColumnName) {
                    builder.push_ir(OpCode::AddColumn, count);
                    if (ast[++count].kind == NodeKind::ColumnType) {
                        builder.push_ir(OpCode::ColumnType, count);
                    } else {
                        return std::unexpected(Errors::SyntaxErr);
                    }
                } else {
                    return std::unexpected(Errors::UnexpectedNode);
                }
                break;
            }
            default: {
                std::cout << count << std::endl;
                break;
            }
        }

        *i = count;
    }

    builder.push_ir(OpCode::EndTable, 0);
    return std::move(builder.ir);
}