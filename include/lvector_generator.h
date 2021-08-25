#pragma once

#include "lvector.h"

namespace lexpr {

template < typename Type,
           std::enable_if_t<std::is_same_v<Type, symbol_t> ||
                            std::is_same_v<Type, number_t>, bool> >
class lvector_generator {
public:
    lvector operator()(std::int32_t system_size, std::int32_t period,
                       std::initializer_list< typename Type::value_type > args) const
    {
        lvector result(system_size, Type::default_value);

        auto value = args.begin();
        for (std::int32_t idx = 0, p_idx = 0; idx < system_size; ++idx, ++p_idx) {
            if (p_idx == period) {
                if (++value == args.end())
                    value = args.begin();
                p_idx = 0;
            }
            result.get_value<Type>(idx) = *value;
        }
        return result;
    }
};

}
