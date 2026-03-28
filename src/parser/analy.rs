use crate::lexer::kinds;
use crate::err::ErrorKinds;
use crate::parser::ast;
use crate::parser::check;
use crate::parser::exprs;


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
    pub fn analysis(
        &self,
        tokens: &Vec<kinds::Token>
    ) -> Result<Vec<ast::Node>, ErrorKinds> {
        let mut pos: usize = 0;
        let checker = exprs::ExprChecker::new(tokens);
        let ast_node = Vec::<ast::Node>::new();

        for i in 0..tokens.len() {
            ast_node.extend(
                match tokens[i].kind {
                    kinds::TokenKind::KeyWordCreate =>
                        checker.create_table(&mut pos),
                    _ => {},
                }
            );
        }

        Ok(self.ast_node.clone())
    }
}