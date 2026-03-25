#[repr(C)]
pub enum AstNodeKind {
    Header,
    OpCreateTable,
    OpInsert,
    OpSelect,
    Column,
    ColumnName,
    ColumnType,
    ColumnNext,
    TableName,
}
#[repr(C)]
pub struct Ast {
    value : usize,
    kind : AstNodeKind,
    left : usize,
    right : usize,
}
