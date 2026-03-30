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
    Name,
    Number,
    Symbol,
    Null,
    Space,
    KeyWordCreate,
    KeyWordTable,
    SymbolLeftParen,
    SymbolRightParen,
    SymbolComma
);

pub type Tokens = Vec<Token>;

impl Token {
    pub fn make_token(value: &Option<String>, kind: TokenKind) -> Self {
        Self {
            value: value.clone(),
            kind: kind,
        }
    }
}