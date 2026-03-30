use crate::parser::*;
use crate::lexer::*;
use crate::err::ErrorKinds;
use crate::id;



fn define_column_node(
    reader: & mut parse_utils::TokenReader
) -> Result<ast::Node, ErrorKinds> {
    Ok(
        ast::Node::attach_node::<{id!(ast::NodeKind::ColumnNode)}>(
        reader.expect_make_node::<
                {id!(TokenKind::Name)},
                {id!(ast::NodeKind::ColumnName)}>()?,
        reader
            .expect_make_node::<
                {id!(TokenKind::Name)},
                {id!(ast::NodeKind::ColumnType)}>()?
        )
    )
}

pub fn parse_stmt(
    reader: &mut parse_utils::TokenReader
) -> Result<(), ErrorKinds> {
    match reader.current_kind::<true>() {
        TokenKind::KeyWordCreate => {
            let table_name: ast::Node 
                = reader
                    .expect_kind::<{
                        id!(TokenKind::KeyWordTable)}>()?
                    .expect_make_node::<
                        {id!(TokenKind::Name)},
                        {id!(ast::NodeKind::TableName)}>()?
                    .expect_kind::<{
                        id!(TokenKind::SymbolLeftParen)}>(reader)?;
            loop {
                let c = define_column_node(reader);
                println!("{:?}", c);
                if reader.check_kind::<{id!(TokenKind::SymbolRightParen)}>() {
                    break;
                }
            }
        }
        _ => {},
    }
    Ok(())
}