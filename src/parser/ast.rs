use crate::define_kinds;
use crate::macros::EnumIdType;

define_kinds!(
    NodeKind,
    TableName = 0,
    ColumnNode = 1,
    ColumnName = 2,
    ColumnType = 3
);

#[derive(Debug, Clone)]
pub struct Node<'a> {
    pub value: String,
    pub kind: NodeKind,
    pub left: Option<&'a Node<'a>>,
    pub right: Option<&'a Node<'a>>,
}


impl<'a> Node<'a> {
    pub fn make<const N: EnumIdType>(value: &String) -> Node<'a> {
        Node {
            value: value.clone(),
            kind: NodeKind::from_u8::<N>().unwrap(),
            left: None,
            right: None,
        }
    }
}