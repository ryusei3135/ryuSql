use crate::define_kinds;

#[derive(Debug, Clone, PartialEq, Copy)]
pub(crate) enum CharKinds {
    Letter,
    Digit,
    Space,
    Symbol,
    Other,
}

#[derive(Debug, Clone, PartialEq)]
pub struct Token {
    pub value: String,
    pub kind: TokenKind,
}

define_kinds!(
    TokenKind,
    Name = 0,
    Number = 1,
    Symbol = 2,
    Null = 3,
    KeyWordCreate = 4,
    KeyWordTable = 5,
    SymbolLeftParen = 6,
    SymbolRightParen = 7,
    SymbolComma = 8
);

pub type Tokens = Vec<Token>;

impl Token {
    pub fn make(value: &String, kind: TokenKind) -> Self {
        Self {
            value: value.clone(),
            kind: kind,
        }
    }
}