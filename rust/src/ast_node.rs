#[repr(C)]
pub enum AstNodeKind {
    OpCreateTable,
    OpInsert,
    OpSelect,
    ColumnName,
    ColumnType,
    TableName,
}
#[repr(C)]
pub struct Ast {
    value : usize,
    kind : AstNodeKind,
    left : usize,
    right : usize,
}
