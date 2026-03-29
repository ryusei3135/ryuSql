use crate::lexer;
use crate::err::ErrorKinds;
use crate::parser::ast;
use crate::macros::EnumIdType;
use crate::id;

pub struct TokenReader<'a> {
    tokens: &'a lexer::Tokens,
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
    ) -> Result<&mut Self, ErrorKinds> {
        if self.check_kind::<T>() && self.increment_index() {
            Ok(self)
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

    pub fn skip_comma(&mut self) -> Result<&mut Self, ErrorKinds> {
        self.expect_kind::<{id!(lexer::TokenKind::SymbolComma)}>()?;
        Ok(self)
    }

    pub fn skip_space(&mut self) -> Result<&mut self, ErrorKinds> {
        self.expect_kind::<{id!(lexer::TokenKind::Space)}>()?;
        Ok(self)
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