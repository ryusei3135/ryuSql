mod lexer;
mod parser;
mod err;
mod macros;

fn main() {
    let mut lexer: lexer::Lexer = lexer::Lexer::new();
    let mut parser = parser::Parser::new();
    lexer.tokenizer("create table name( )");
    parser.analysis(&lexer.tokens);
    println!("{:?}", lexer.tokens);
}


#[cfg(test)]
mod tests {
    use crate::lexer::*;

    #[test]
    fn check_keyword_token() {
        let mut lexer = Lexer::new();
        lexer.tokenizer("CREATE create");
        let tokens: &Vec<kinds::Token> = &lexer.tokens;
        assert_eq!(tokens[0].kind, syntax::TokenKind::KeyWordCreate);
        assert_eq!(tokens[2].kind, syntax::TokenKind::KeyWordCreate);
    }
}