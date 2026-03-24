#include "analy.hpp"
#include "char_table.tpp"
#include "RyuSql.hpp"


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

    Opt::ErrOpt stack_char(
        const char chr, 
        const Analy::CharKinds kind
    ) {
        const ReturnTokenKind result = categorize_token_kind(value, last_kind);
        if (!result.has_value()) {
            last_kind = kind;
            return result.error();
        }

        if (check_stackable(kind)) {
            tokens.push_back(Token {value, result.value().value()});
            value.clear();
        }

        if (kind != Analy::CharKinds::Space)
            value.push_back(chr);
        this->last_kind = kind;
        return std::nullopt;
    }

    Opt::ErrOpt exit() {
        const ReturnTokenKind result = categorize_token_kind(value, last_kind);
        if (!result.has_value())
            return result.error();
        if (!result.value().has_value())// 空白は無視する
            return std::nullopt;

        if (value.length()) {
            tokens.push_back(Token {value, result.value().value()});
            value.clear();
        }
        return std::nullopt;
    }
private:
    std::string value;
    Analy::CharKinds last_kind = Analy::CharKinds::NONE;
    char last_char = '\0';
    
    inline bool check_stackable(
        const Analy::CharKinds kind
    ) {
        return 
            (value.length()
            && last_kind != kind
            && (kind != Analy::CharKinds::Symbol || last_kind != Analy::CharKinds::Symbol)
            && last_kind != Analy::CharKinds::NONE
            && last_kind != Analy::CharKinds::Space)
            || (kind == Analy::CharKinds::Symbol && last_kind == kind);
    }

    inline ReturnTokenKind categorize_token_kind(
        const std::string& token, 
        const Analy::CharKinds kind
    ) {
        switch (kind) {
            case Analy::CharKinds::Digit: return TokenKind::NUMBER;
            case Analy::CharKinds::Symbol: {
                if (token.length() == 1) {
                    const TokenKind token_kind
                        = Table::init_char_table<TokenKind>()[token[0]];
                    if (token_kind != TokenKind::Null) {
                        return token_kind;
                    } else {
                        #ifdef DEBUG
                        std::cerr
                            << "[lexer:err]: invalid token kind.; "
                            << "[func:categorize_token_kind]: Symbol"
                            << std::endl;
                        #endif
                        return std::unexpected(Errors::InvalidTokenKind);
                    }
                } else { // この長さの記号の文字列はトークンで使えません。
                    #ifdef DEBUG
                    std::cerr 
                        << "[lexer:err]: This string token cannot be used.; "
                        << "[func:categorize_token_kind]: Symbol"
                        << std::endl;
                    #endif
                    return std::unexpected(Errors::InvalidString);
                }
            }
            case Analy::CharKinds::Letter: {
                switch (hash(token)) {
                    case hash("INT"):
                        return TokenKind::ColumnType;
                    case hash("TEXT"):
                        return TokenKind::ColumnType;
                    case hash("FROM"): 
                        return TokenKind::FROM;
                    case hash("TABLE"):
                        return TokenKind::TABLE;
                    case hash("SELECT"):
                        return TokenKind::SELECT;
                    case hash("CREATE"): 
                        return TokenKind::CREATE;
                    case hash("INSERT"):
                        return TokenKind::INSERT;
                    case hash("INTO"):
                        return TokenKind::INTO;
                    case hash("VALUES"):
                        return TokenKind::VALUES;
                }
                return TokenKind::STRING;
            }
            case Analy::CharKinds::Space: 
                return std::nullopt;
            case Analy::CharKinds::NONE:
                return std::nullopt;
            default: {
                #ifdef DEBUG
                std::cerr
                    << "[lexer:err]: invalid token kind;"
                    << std::endl;
                #endif
                return std::unexpected(Errors::InvalidTokenKind);
            }
        }
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

std::expected<std::vector<Token>, Errors>
Analy::input_sql_query(const char* ascii_sql_query) {
    Stack token_stack = {};
    size_t size = strlen(ascii_sql_query);
    size_t query_ptr = 0;
    uint8_t loop_count = 8;
    const std::array<CharKinds, 256> char_table = Table::init_char_table<CharKinds>();

    while (query_ptr < size) {
        size_t chunk
            = query_ptr + 8 > size
            ? load::load64_safe(
                ascii_sql_query + query_ptr, 
                loop_count = size - query_ptr)
            : load::load64(ascii_sql_query + query_ptr);

        for(int i = 0;i < loop_count; i++) {
            uint8_t c = (chunk >> (i*8)) & 0xff;
            ErrTry(token_stack.stack_char(c, char_table[c]));
        }

        query_ptr += loop_count;
    }

    ErrTry(token_stack.exit());
    return token_stack.tokens; 
}