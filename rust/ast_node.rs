#[repr(C)]
struct Ast {
    value : usize,

    kind : AstNodeKind,

    left : usize,

    right : usize,

}
