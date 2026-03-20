#include "analy.hpp"


inline TokenKind categorize_token_kind(std::string token, CharKinds kind) {
    const std::array<TokenKind, 256> table = init_char_table<TokenKind>();
    if (kind == CharKinds::Digit) {
        return TokenKind::NUMBER;
    } else if (kind == CharKinds::Symbol) {
        if (token.length() == 1) {
            TokenKind token_kind = table[token[0]];
            if (token_kind != TokenKind::Null) {
                return token_kind;
            } else {
                std::cerr << "invaild token kind:" << token[0] << std::endl;
            }
        }
    }

    switch (token.length()) {
        case 3: {
            if (token == "INT") {
                return TokenKind::ColumnType;
            }
            break;
        }
        case 4: {
            if (token == "FROM") {
                return TokenKind::FROM;
            } else if (token == "TEXT") {
                return TokenKind::ColumnType;
            }
            break;
        }
        case 5: {
            if (token == "TABLE")
                return TokenKind::TABLE;
            break;
        }
        case 6: {
            if (token == "SELECT") {
                return TokenKind::SELECT;
            } else if (token == "CREATE") {
                return TokenKind::CREATE;
            }
            break;
        }
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