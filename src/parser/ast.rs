use crate::parser::ast::NodeKind;

pub enum NodeKind {
    TableName(String),
    ColumnNode(NodeKind::ColumnName, NodeKind::ColumnType),
    ColumnName(String),
    ColumnType(String)
};

#[derive(Debug, Clone)]
pub struct Node<'a> {
    pub value: String,
    pub kind: NodeKind,
    pub left: Option<&'a Node<'a>>,
    pub right: Option<&'a Node<'a>>,
}


impl<'a> Node<'a> {
    pub fn make<K>(value: &String, node_kind: NodeKind) -> Node<'a> where K: IsNodeKind {
        Node {
            value: value.clone(),
            kind: node_kind,
            left: None,
            right: None,
        }
    }
}