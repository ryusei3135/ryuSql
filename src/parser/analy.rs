use crate::lexer;
use crate::err::ErrorKinds;
use crate::parser::*;


pub struct Parser<'a> {
    ast_node: Vec<ast::Node<'a>>,
}

impl Parser<'_> {
    pub fn new() -> Self {
        Self {
            ast_node: Vec::<ast::Node>::new(),
        }
    }
    /// トークンの配列をastに変換
    pub fn analysis(&self, tokens: &lexer::Tokens) -> Result<Vec<ast::Node>, ErrorKinds> {
        let mut index: usize = 0;
        let mut token_reader = 
            parse_utils::TokenReader::new(tokens, &mut index);

        let _ = stmt::parse_stmt(&mut token_reader);

        Ok(self.ast_node.clone())
    }
}