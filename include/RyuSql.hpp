#pragma once


#include <string>
#include <iostream>
#include <cstdint>
#include <vector>
#include <array>
#include <concepts>
#include <utility>
#include <cstring>
#include <expected>
#include <optional>

#include "analy.hpp"
#include "error.hpp"


#define DEBUG true

namespace _local {
    template<typename T>
    struct is_expected : std::false_type {};
    template<typename T, typename E>
    struct is_expected<std::expected<T, E>> : std::true_type {};

    template<typename T>
    concept is_expected_v =
        is_expected<std::decay_t<T>>::value;

    
    template<typename T>
    struct is_optional : std::false_type {};
    template<typename T>
    struct is_optional<std::optional<T>> : std::true_type {};

    template<typename T>
    concept is_optional_v =
        is_optional<std::decay_t<T>>::value;

    // expectedやoptionalがエラーの場合エラーだけ取り出す
    template<typename T>
    inline std::optional<Errors> _is_err(const T result) {
        if constexpr (_local::is_expected_v<T>) {
            if (!result.has_value())
                return result.error();
            if constexpr (std::is_same_v<decltype(result.value()), Errors>)
                return result.value();
            return std::nullopt;
        } else if constexpr (_local::is_optional_v<T>) {
            if (result.has_value())
                return result.value();
            return std::nullopt;
        } else { // この関数では、extectedとoptional以外は使えない
            static_assert(false, "[code: CondReturn macro]この型は使えません");
        }
    }
};

#define ErrTry(result)\
    using result_type = std::decay_t<decltype((result))>;\
    if (std::optional<Errors> err \
        = _local::_is_err<result_type>((result))) {\
        return std::unexpected(err.value());\
    }
