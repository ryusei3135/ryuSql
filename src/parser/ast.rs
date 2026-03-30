use crate::define_kinds;
use crate::err::ErrorKinds;
use crate::macros::EnumIdType;
use crate::parser::parse_utils;

define_kinds!(
    NodeKind,
    StmtCrateTable,
    TableName,
    ColumnNode,
    ColumnName,
    ColumnType
);

#[derive(Debug, Clone)]
pub struct Node {
    pub value: Option<String>,
    pub kind: NodeKind,
    pub left: Option<Box<Node>>,
    pub right: Option<Box<Node>>,
}


impl Node {
    pub fn make<const N: EnumIdType>(value: &String) -> Self {
        Self {
            value: Some(value.clone()),
            kind: NodeKind::from_u8::<N>().unwrap(),
            left: None,
            right: None,
        }
    }

    pub fn attach_node<const N: EnumIdType>(left: Node, right: Node) -> Self {
        Self {
            value: None,
            kind: NodeKind::from_u8::<N>().unwrap(),
            left: Some(Box::new(left)),
            right: Some(Box::new(right))
        }
    }

    pub fn expect_kind<const T: EnumIdType>(
        self, 
        reader: &mut parse_utils::TokenReader
    ) -> Result<Self, ErrorKinds> {
        reader.expect_kind::<T>()?;
        Ok(self)
    }
}