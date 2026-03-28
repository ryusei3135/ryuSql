use crate::parser::{ast, check};
use crate::lexer::kinds;
use crate::err::ErrorKinds;


pub struct ExprChecker<'a> {
    check_token: &'a check::CheckTokenKind<'a>,
    ast_node: Vec<ast::Node>,
}

impl<'a> ExprChecker<'a> {
    pub fn new<'b>(tokens: &'a Vec<kinds::Token>) -> Self {
        Self {
            check_token: &check::CheckTokenKind::new(tokens),
            ast_node: Vec::<ast::Node>::new(),
        }
    }

    pub fn create_table(&self, pos: &mut usize) -> Result<&Vec<kinds::Token>, ErrorKinds> {
        self.check_token.consume(kinds::TokenKind::KeyWordCreate, pos)?;
        self.check_token.consume(kinds::TokenKind::KeyWordTable, pos)?;

        self.check_token.consume_make_node::<
            kinds::NameType,
            ast::TableNameType
        >(pos);
        Ok(&self.ast_node)
    }
}