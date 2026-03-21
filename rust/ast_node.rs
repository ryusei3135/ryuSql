

#[repr(C)]
enum AstNodeKind {
    OpCreateTable,
    OpInsert,
    OpSelect,
    ColumnName,
    ColumnType,
    TableName,
} 