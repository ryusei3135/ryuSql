use crate::lexer::kinds;
use crate::err::ErrorKinds;
use crate::parser::ast;


pub struct TokenReader<'a> {
    tokens: &'a Vec<kinds::Token>,
    index: &'a usize
}

impl<'a> TokenReader<'a> {
    pub fn new(tokens: &'a Vec<kinds::Token>, index: &'a usize) -> Self {
        Self {
            tokens,
            index
        }
    }

    pub fn expect_kind<TOKEN>(
        &mut self,
    ) -> Result<(), ErrorKinds> {
        if self.check_kind::<TOKEN>() && self.increment_index() {
            Ok(())
        } else {
            Err(ErrorKinds::UnexpectedTokenKind)
        }
    }

    pub fn expect_make_node<TOKEN, NODE>(
        &mut self, 
    ) -> Result<ast::Node, ErrorKinds>
    where TOKEN: u8, NODE: u8 {
        if self.check_kind::<TOKEN>() && self.increment_index() {
            Ok(ast::Node::make<NODE>(value))
        } else {
            Err(ErrorKinds::UnexpectedTokenKind)
        }
    }

    #[inline(analy)]
    fn check_kind<TOKEN>(&self) -> bool where TOKEN: u8 {
        self.tokens[self.index].kind == kinds::TokenKind::from_kind::<TOKEN>()
    }

    #[inline(analy)]
    fn increment_index(&mut self) -> bool {
        (self.index += 1) != 0
    }
}