mod lexer;
mod parser;
mod err;
mod macros;

fn main() {
    let mut lexer: lexer::Lexer = lexer::Lexer::new();
    let parser = parser::Parser::new();
    lexer.tokenizer("create table name(n INT)");
    println!("{:?}", &lexer.tokens);
    let _ = parser.analysis(lexer.tokens);
}


#[cfg(test)]
mod tests {
    use crate::lexer::{self, *};
    use crate::parser::analy::Parser;

    #[test]
    fn check_keyword_token() {
        let mut lexer = Lexer::new();
        lexer.tokenizer("CREATE create");
        let tokens: &Vec<lexer::Token> = &lexer.tokens;
        assert_eq!(tokens[0].kind, lexer::TokenKind::KeyWordCreate);
        assert_eq!(tokens[2].kind, lexer::TokenKind::KeyWordCreate);
    }

    #[test]
    fn check_stmt() {
        let mut lexer = Lexer::new();
        lexer.tokenizer("CREATE TABLE name(name INT)");
        let mut parser = Parser::new();
        let _ = parser.analysis(lexer.tokens);
    }
}