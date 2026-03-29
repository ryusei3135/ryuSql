use crate::parser::*;
use crate::lexer::*;
use crate::err::ErrorKinds;
use crate::id;


pub fn parse_stmt(
    token_reader: &mut parse_utils::TokenReader
) -> Result<(), ErrorKinds> {
    match token_reader.current_kind::<true>() {
        TokenKind::KeyWordCreate => {
            token_reader.expect_kind::<{id!(TokenKind::KeyWordTable)}>()?;
            let table_name = token_reader.expect_make_node::<
                {id!(TokenKind::Name)},
                {id!(ast::NodeKind::TableName)}>()?;
            token_reader.expect_kind::< // "("を期待
                {id!(TokenKind::SymbolLeftParen)}>()?;
            loop {
                let column_name = token_reader.expect_make_node::<
                    {id!(TokenKind::Name)},
                    {id!(ast::NodeKind::ColumnName)}>()?;
                token_reader.expect_kind::<{id!(TokenKind::SymbolComma)}>()?;
                if token_reader.check_kind::<{id!(TokenKind::SymbolRightParen)}>() {
                    break;
                }
            }
        }
        _ => {},
    }
    Ok(())
}