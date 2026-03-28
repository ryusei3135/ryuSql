use crate::lexer::kinds;
use crate::err::ErrorKinds;
use crate::parser::ast;


pub struct CheckTokenKind<'a> {
    tokens: &'a Vec<kinds::Token>
}

impl<'a> CheckTokenKind<'a> {
    pub fn new(tokens: &'a Vec<kinds::Token>) -> Self {
        Self {
            tokens,
        }
    }

    pub fn consume(
        &self, 
        kind: kinds::TokenKind,
        pos: &mut usize
    ) -> Result<(), ErrorKinds> {
        if self.tokens[*pos].kind == kind {
            *pos += 1;
            Ok(())
        } else {
            Err(ErrorKinds::UnexpectedTokenKind)
        }
    }

    pub fn consume_make_node<T>(
        &self, 
        node_kind: ast::NodeKind,
        pos: &mut usize
    ) -> Result<ast::Node, ErrorKinds>
    where
        T: kinds::IsTokenKind,
        N: ast::IsNodeKind
    {
        if self.tokens[*pos].kind == T::VALUE {
            *pos += 1;
            Ok(ast::Node::make(&self.tokens[*pos - 1].value, node_kind))
        } else {
            Err(ErrorKinds::UnexpectedTokenKind)
        }
    }
}