#include "analy.hpp"


// テーブルの初期化
namespace Table {
    template<typename T>
    concept EnumOnly = std::is_enum_v<T>;

    template<EnumOnly T>
    consteval const std::array<T, 256> init_char_table() {
        using FillValues = std::vector<std::pair<std::array<char, 2>, T>>;
        auto fill_table = [&](auto& table, std::array<char, 2> range, T value) {
            for (int i = range[0]; i <= range[1]; i++)
                table[i] = value;
        };
        std::array<T, 256> table{};

        if constexpr (std::is_same_v<T, Analy::CharKinds>) {
            FillValues fill_values_list = {
                {{(char)0, (char)255}, Analy::CharKinds::Other},
                {{(char)9, (char)13}, Analy::CharKinds::Space},
                {{'!', '/'}, Analy::CharKinds::Symbol},
                {{':', '@'}, Analy::CharKinds::Symbol},
                {{'A', 'Z'}, Analy::CharKinds::Letter},
                {{'a', 'z'}, Analy::CharKinds::Letter},
                {{'0', '9'}, Analy::CharKinds::Digit}
            };
            for (auto [range, fill_value]: fill_values_list)
                fill_table(table, range, fill_value);
            table[' '] = Analy::CharKinds::Space;
        } else {
            fill_table(table, std::array{(char)0, (char)255}, TokenKind::Null);
            table['('] = TokenKind::LeftParen;
            table[')'] = TokenKind::RightParen;
            table[','] = TokenKind::Comma;
            table[';'] = TokenKind::Semicolon;
        }

        return table;
    }
};