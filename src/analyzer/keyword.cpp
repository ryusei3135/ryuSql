#include "analy.hpp"


constexpr uint32_t hash(std::string target) {
    uint32_t h = 0;
    for (char c : target) {
        h = h * 31 + c;
    }
    return h;
}

inline TokenKind categorize_token_kind(std::string token, CharKinds kind) {
    if (kind == CharKinds::Digit) {
        return TokenKind::NUMBER;
    } else if (kind == CharKinds::Symbol) {
        if (token.length() == 1) {
            const TokenKind token_kind
                = init_char_table<TokenKind>()[token[0]];
            if (token_kind != TokenKind::Null) {
                return token_kind;
            } else {
                std::cerr << "invaild token kind:" << token[0] << std::endl;
            }
        }
    }

    switch (hash(token)) {
        case hash("INT"): return TokenKind::ColumnType;
        case hash("TEXT"): return TokenKind::ColumnType;
        case hash("FROM"): return TokenKind::FROM;
        case hash("TABLE"): return TokenKind::TABLE;
        case hash("SELECT"): return TokenKind::SELECT;
        case hash("CREATE"): return TokenKind::CREATE;
    }

    return TokenKind::STRING;
}

inline bool check_stackable(
    const Stack& self, 
    CharKinds kind
) {
    return 
        (self.value.length()
        && self.last_kind != kind
        && (kind != CharKinds::Symbol || self.last_kind != CharKinds::Symbol)
        && self.last_kind != CharKinds::NONE
        && self.last_kind != CharKinds::Space)
        || kind == CharKinds::Symbol && self.last_kind == kind;
}

void Stack::stack_char(const char chr, CharKinds kind) {
    if (check_stackable(*this, kind)) {
        tokens.push_back(
            Token {
                value,
                categorize_token_kind(value, last_kind),
            }
        );

        value.clear();
    }

    if (kind != CharKinds::Space) {
        value.push_back(chr);
    }
    this->last_kind = kind;
}

void Stack::exit() {
    if (value.length()) {
        tokens.push_back(
            Token {
                value,
                categorize_token_kind(value, last_kind),
            }
        );

        value.clear();
    }
}