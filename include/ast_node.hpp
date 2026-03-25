#pragma once

#include <string>

enum class AstNodeKind {
    Header,
    OpCreateTable,
    OpInsert,
    OpSelect,
    Column,
    ColumnName,
    ColumnType,
    ColumnNext,
    TableName,
};
struct Ast {
    size_t  value;
    AstNodeKind  kind;
    size_t  left;
    size_t  right;
};
