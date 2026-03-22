#pragma once

#include <string>

enum class AstNodeKind {
    OpCreateTable,
    OpInsert,
    OpSelect,
    ColumnName,
    ColumnType,
    TableName,
};
struct Ast {
    size_t  value;
    AstNodeKind  kind;
    size_t  left;
    size_t  right;
};
