#[repr(C)]
enum TokenKind {
    FROM,
    TABLE,
    SELECT,
    CREATE,
    STRING,
    NUMBER,
    INSERT,
    INTO,
    VALUES,
    LeftParen,
    RightParen,
    Comma,
    Semicolon,
    ColumnType,
    Null,
}
#[repr(C)]
struct Token {
    value : String,

    token_kind : TokenKind,

}
