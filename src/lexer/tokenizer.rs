use crate::lexer;

pub struct Lexer {
    last_char_kinds: Option<lexer::CharKinds>,
    stack_chars: Option<String>,
    pub tokens: Vec<lexer::Token>,
}
// check_reserved_word
impl Lexer {
    pub fn new() -> Self {
        Self {
            last_char_kinds: None,
            stack_chars: None,
            tokens: Vec::<lexer::Token>::new(),
        }
    }

    pub fn tokenizer(&mut self, text: &str) {
        for chr in text.chars() {
            let char_kinds: lexer::CharKinds = match chr {
                'a'..='z' | 'A'..='Z' => lexer::CharKinds::Letter,
                '0'..='9' => lexer::CharKinds::Digit,
                ' ' | '\t' | '\n' => lexer::CharKinds::Space,
                '!'..='/' | ':'..='@' | '['..='`' | '{'..='~' =>
                    lexer::CharKinds::Symbol,
                _ => lexer::CharKinds::Other,
            };
            self.check_stackable(&char_kinds);
            self.stacking_chars(&char_kinds, &chr);
        }
        self.push_token();
    }
    /// トークンがスタック可能か調べる
    /// スタック可能なら現在スタックされている文字列をトークンに変換する
    /// スタックできないなら何もしない
    #[inline(always)]
    fn check_stackable(&mut self, char_kinds: &lexer::CharKinds) {
        if let Some(last_chr_kind) = self.last_char_kinds {
            if char_kinds.clone() != last_chr_kind {
                self.push_token();
                self.stack_chars = None;
            }
        }
    }

    fn push_token(&mut self) {
        self.tokens.push(
            lexer::Token::make_token(
                &self.stack_chars,
                identify_token_kind(
                    &self.stack_chars.as_ref().unwrap(),
                    &self.last_char_kinds.unwrap(), 
                )
            )
        );
    }
    /// 文字をスタックする
    fn stacking_chars(&mut self, char_kinds: &lexer::CharKinds, chr: &char) {
        if let Some(ref mut s) = self.stack_chars {
            s.push_str(&chr.clone().to_string());
        } else {
            self.stack_chars = Some(chr.clone().to_string());
        }
        self.last_char_kinds = Some(char_kinds.clone());
    }
}


fn identify_token_kind(
    stack_char: &str,
    chr_kind: &lexer::CharKinds
) -> lexer::TokenKind {
    return match (chr_kind, stack_char) {
        (lexer::CharKinds::Digit, _) => lexer::TokenKind::Number,
        (lexer::CharKinds::Letter, _) =>
            lookup_keyword(&stack_char)
            .unwrap_or(lexer::TokenKind::Name),
        (lexer::CharKinds::Symbol, _) =>
            read_symbol(&stack_char)
            .unwrap_or(lexer::TokenKind::Symbol),
        (_, _) => lexer::TokenKind::Null
    };
}


/// 文字列がキーワードか、調べる関数
/// # 引数
/// - keyword
///     調べる対象の文字列
/// # 戻り値
/// - キーワードに属さない場合のみNone
/// - 文字列がアルファベットでない場合もNone
fn lookup_keyword(
    keyword: &str
) -> Option<lexer::TokenKind> {
    if keyword.chars().all(|c| c.is_ascii_alphabetic()) {
        return Some(
            match keyword.to_uppercase().as_str() {
                "CREATE" => lexer::TokenKind::KeyWordCreate,
                "TABLE" => lexer::TokenKind::KeyWordTable,
                _ => {
                    return None;
                }
            }
        );
    }
    None
}

/// 記号をトークンの種類に分ける関数
/// # 引数
/// - symbol
///     調べる対象の記号
/// # 戻り値
/// - 記号が登録されていない場合None
fn read_symbol(
    symbol: &str
) -> Option<lexer::TokenKind> {
    return Some(
        match symbol {
            "(" => lexer::TokenKind::SymbolLeftParen,
            ")" => lexer::TokenKind::SymbolRightParen,
            "," => lexer::TokenKind::SymbolComma,
            _ => return None,
        }
    );
}