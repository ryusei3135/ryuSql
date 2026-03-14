#include "analy.hpp"


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

constexpr std::array<CharKinds, 256> init_char_kind_table() {
    std::array<CharKinds, 256> table{};

    for (int i = 0; i < 256; i++)
        table[i] = CharKinds::Other;

    table[' '] = CharKinds::Space;
    table['\t'] = CharKinds::Space;
    table['\n'] = CharKinds::Space;

    table['('] = CharKinds::Symbol;
    table[')'] = CharKinds::Symbol;
    table[','] = CharKinds::Symbol;
    table[';'] = CharKinds::Symbol;

    for (int i = 'a'; i <= 'z'; i++)
        table[i] = CharKinds::Letter;

    for (int i = 'A'; i <= 'Z'; i++)
        table[i] = CharKinds::Letter;

    for (int i = '0'; i <= '9'; i++)
        table[i] = CharKinds::Digit;

    return table;
}

std::vector<Token> input_sql_query(char* ascii_sql_query) {
    Stack token_stack = {};
    size_t size = strlen(ascii_sql_query);
    size_t query_ptr = 0;
    uint8_t loop_count = 8;
    std::array<CharKinds,256> char_table = init_char_kind_table();

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