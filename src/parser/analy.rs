use crate::lexer;
use crate::err::ErrorKinds;
use crate::parser::*;


pub struct Parser {
    ast_node: Vec<ast::Node>,
}

impl Parser {
    pub fn new() -> Self {
        Self {
            ast_node: Vec::<ast::Node>::new(),
        }
    }
    /// トークンの配列をastに変換
    pub fn analysis(
        &self, 
        mut tokens: lexer::Tokens
    ) -> Result<Vec<ast::Node>, ErrorKinds> {
        let mut index: usize = 0;
        let mut token_reader = 
            parse_utils::TokenReader::new(&mut tokens, &mut index);

        let _ = stmt::parse_stmt(&mut token_reader);

        Ok(self.ast_node.clone())
    }
}