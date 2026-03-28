use crate::lexer::kinds;

pub struct Lexer {
    last_char_kinds: Option<kinds::CharKinds>,
    stack_chars: Option<String>,
    pub tokens: Vec<kinds::Token>,
}
// check_reserved_word
impl Lexer {
    pub fn new() -> Self {
        Self {
            last_char_kinds: None,
            stack_chars: None,
            tokens: Vec::<kinds::Token>::new(),
        }
    }

    pub fn tokenizer(&mut self, text: &str) {
        for chr in text.chars() {
            let char_kinds: kinds::CharKinds = match chr {
                'a'..='z' | 'A'..='Z' => kinds::CharKinds::Letter,
                '0'..='9' => kinds::CharKinds::Digit,
                ' ' | '\t' | '\n' => kinds::CharKinds::Space,
                '!'..='/' | ':'..='@' | '['..='`' | '{'..='~' =>
                    kinds::CharKinds::Symbol,
                _ => kinds::CharKinds::Other,
            };
            self.check_stackable(&char_kinds);
            self.stacking_chars(&char_kinds, &chr);
        }
        self.push_token();
    }

    fn check_stackable(&mut self, char_kinds: &kinds::CharKinds) {
        if self.last_char_kinds.is_some() {
            if char_kinds.clone() != self.last_char_kinds.unwrap() {
                self.push_token();
                self.stack_chars = None;
            }
        }
    }

    fn push_token(&mut self) {
        self.tokens.push(
            kinds::Token::make(
                &self.stack_chars.clone().unwrap(),
                identify_token_kind(
                    &self.stack_chars.as_ref().unwrap(),
                    &self.last_char_kinds.unwrap(), 
                )
            )
        );
    }

    fn stacking_chars(&mut self, char_kinds: &kinds::CharKinds, chr: &char) {
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
    chr_kind: &kinds::CharKinds
) -> kinds::TokenKind {
    return match (chr_kind, stack_char) {
        (kinds::CharKinds::Digit, _) => kinds::TokenKind::Number,
        (kinds::CharKinds::Letter, _) =>
            lookup_keyword(&stack_char)
            .unwrap_or(kinds::TokenKind::Name),
        (kinds::CharKinds::Symbol, _) =>
            read_symbol(&stack_char)
            .unwrap_or(kinds::TokenKind::Symbol),
        (_, _) => kinds::TokenKind::NONE
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
) -> Option<kinds::TokenKind> {
    if keyword.chars().all(|c| c.is_ascii_alphabetic()) {
        return Some(
            match keyword.to_uppercase().as_str() {
                "CREATE" => kinds::TokenKind::KeyWordCreate,
                "TABLE" => kinds::TokenKind::KeyWordTable,
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
) -> Option<kinds::TokenKind> {
    return Some(
        match symbol {
            "(" => kinds::TokenKind::SymbolLeftParen,
            ")" => kinds::TokenKind::SymbolRightParen,
            "," => kinds::TokenKind::SymbolComma,
            _ => return None,
        }
    );
}