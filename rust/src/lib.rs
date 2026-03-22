mod ast_node;
mod token;

// #[no_mangle]
// pub extern "C" fn create_table_node(
//     tokens_data: *const token::Token,
//     len: usize,
//     count: *mut usize
// ) {
//     let slice = unsafe { std::slice::from_raw_parts(tokens_data, len) };

//     for x in slice {
//         println!("{}", x);
//     }
// }