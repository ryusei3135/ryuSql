use crate::define_enum;

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

define_enum!(
    TokenKind,
    Name,
    Number,
    Symbol,
    NONE,
    KeyWordCreate,
    KeyWordTable,
    SymbolLeftParen,
    SymbolRightParen,
    SymbolComma
);

impl Token {
    pub fn make(value: &String, kind: TokenKind) -> Self {
        Self {
            value: value.clone(),
            kind: kind,
        }
    }
}