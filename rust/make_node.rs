mod ast_node;
mod token;

#[no_mangle]
pub extern "C" fn create_table_node(
    tokens: *const token::Token,
    len: usize
) {
    let slice = unsafe { std::slice::from_raw_parts(tokens, len) };

    for x in slice {
        println!("{}", x);
    }
}