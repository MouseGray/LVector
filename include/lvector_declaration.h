#pragma once

#include <type_traits>
#include <cstdint>

namespace lexpr {

enum class lvector_type : bool {
    symbol,
    number
};

struct symbol_t {
    using value_type = bool;
    static constexpr value_type default_value = false;
    static constexpr lvector_type type_value = lvector_type::symbol;
};

struct number_t {
    using value_type = double;
    static constexpr value_type default_value = 0.0;
    static constexpr lvector_type type_value = lvector_type::number;
};

union lvector_value {
    symbol_t::value_type symbol;
    number_t::value_type number;
    lvector_value(symbol_t::value_type val) { symbol = val; }
    lvector_value(number_t::value_type val) { number = val; }
};

}
