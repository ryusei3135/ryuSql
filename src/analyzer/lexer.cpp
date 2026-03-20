#include "analy.hpp"


constexpr uint32_t hash(std::string target) {
    uint32_t h = 0;
    for (char c : target) {
        h = h * 31 + c;
    }
    return h;
}

class Stack {
public:
    std::vector<Token> tokens;

    void stack_char(const char chr, CharKinds kind) {
        if (check_stackable(kind)) {
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
    
    void exit() {
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
private:
    std::string value;
    CharKinds last_kind = CharKinds::NONE;
    char last_char = '\0';
    
    inline bool check_stackable(
        CharKinds kind
    ) {
        return 
            (value.length()
            && last_kind != kind
            && (kind != CharKinds::Symbol || last_kind != CharKinds::Symbol)
            && last_kind != CharKinds::NONE
            && last_kind != CharKinds::Space)
            || kind == CharKinds::Symbol && last_kind == kind;
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
};

namespace load {
    size_t load64_safe(const char* p, size_t remaining) {
        size_t v = 0;
        memcpy(&v, p, remaining);
        return v;
    }

    size_t load64(const char* p) {
        size_t v = 0;
        memcpy(&v, p, 8);
        return v;
    }
}

std::vector<Token> input_sql_query(const char* ascii_sql_query) {
    Stack token_stack = {};
    size_t size = strlen(ascii_sql_query);
    size_t query_ptr = 0;
    uint8_t loop_count = 8;
    const std::array<CharKinds, 256> char_table = init_char_table<CharKinds>();

    while (query_ptr < size) {
        size_t chunk
            = query_ptr + 8 > size
            ? load::load64_safe(
                ascii_sql_query + query_ptr, 
                loop_count = size - query_ptr)
            : load::load64(ascii_sql_query + query_ptr);

        for(int i = 0;i < loop_count; i++) {
            uint8_t c = (chunk >> (i*8)) & 0xff;
            token_stack.stack_char(c, char_table[c]);
        }

        query_ptr += loop_count;
    }

    token_stack.exit();
    return token_stack.tokens;
}