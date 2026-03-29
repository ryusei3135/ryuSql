use crate::lexer;
use crate::err::ErrorKinds;
use crate::parser::ast;
use crate::macros::EnumIdType;


pub struct TokenReader<'a> {
    tokens: &'a Vec<lexer::Token>,
    index: &'a mut usize
}

impl<'a> TokenReader<'a> {
    pub fn new(tokens: &'a lexer::Tokens, index: &'a mut usize) -> Self {
        Self {
            tokens,
            index
        }
    }
    /// トークンの種類を比較し一致しなければエラーを返す
    pub fn expect_kind<const T: EnumIdType>(
        &mut self,
    ) -> Result<(), ErrorKinds> {
        if self.check_kind::<T>() && self.increment_index() {
            Ok(())
        } else {
            Err(ErrorKinds::UnexpectedTokenKind)
        }
    }
    /// トークンの種類を比較し一致すればノードを作成し返す
    pub fn expect_make_node<const T: EnumIdType, const N: EnumIdType>(
        &mut self, 
    ) -> Result<ast::Node, ErrorKinds> {
        self.expect_kind::<T>()?;
        Ok(ast::Node::make::<N>(&self.tokens[*self.index].value))
    }
    #[inline(always)]
    pub fn current_kind<const I: bool>(&mut self) -> lexer::TokenKind {
        self.tokens[
            if I {
                *self.index += 1;
                *self.index - 1
            } else {
                *self.index
            }
        ].kind
    }

    pub fn not_token<const T: EnumIdType>(&self) -> bool {
        self.tokens[*self.index].kind
            != lexer::TokenKind::from_u8::<T>()
                .unwrap()
    }
    /// トークンの種類を比較する
    #[inline(always)]
    pub fn check_kind<const T: EnumIdType>(&self) -> bool {
        self.tokens[*self.index].kind
            == lexer::TokenKind::from_u8::<T>()
                .unwrap()
    }

    #[inline(always)]
    fn increment_index(&mut self) -> bool {
        if *self.index + 1 < self.tokens.len() {
            *self.index += 1;
            true
        } else {
            false
        }
    }
}