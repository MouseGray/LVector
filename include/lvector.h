#pragma once

#include <stdexcept>
#include <vector>
#include <string>

#include "lvector_declaration.h"

namespace lexpr {

template < typename Type,
           std::enable_if_t<std::is_same_v<Type, symbol_t> ||
                            std::is_same_v<Type, number_t>, bool> = true >
class lvector_generator;

class lvector
{
    friend class lvector_generator<symbol_t, true>;
    friend class lvector_generator<number_t, true>;
public:
    lvector(std::int32_t system_size) :
        lvector(system_size, symbol_t::default_value) {}

    lvector(std::int32_t system_size, symbol_t::value_type val) :
        lvector(system_size, lvector_value{val}, symbol_t::type_value) {}

    lvector(std::int32_t system_size, number_t::value_type val) :
        lvector(system_size, lvector_value{val}, number_t::type_value) {}

    lvector(std::int32_t system_size, lvector_value val, lvector_type type) :
        _data(system_size, val), _type(type) {}

    bool operator==(const lvector& val) const noexcept;

    inline bool operator!=(const lvector& val) const noexcept
    {
        return !(*this == val);
    }

    lvector operator~() const;
    lvector operator|(const lvector& val) const;
    lvector operator&(const lvector& val) const;
    lvector operator^(const lvector& val) const;
    lvector operator+(const lvector& val) const;
    lvector operator-() const;
    lvector operator-(const lvector& val) const;
    lvector operator*(const lvector& val) const;
    lvector operator/(const lvector& val) const;

    lvector& operator|=(const lvector& val);
    lvector& operator&=(const lvector& val);
    lvector& operator^=(const lvector& val);
    lvector& operator+=(const lvector& val);
    lvector& operator-=(const lvector& val);
    lvector& operator*=(const lvector& val);
    lvector& operator/=(const lvector& val);

    inline std::int32_t system_size() const noexcept
    {
        return static_cast<std::int32_t>(_data.size());
    }

    lvector& to_quantity();

    std::string to_string() const;
protected:
    template<typename Ty>
    typename Ty::value_type& get_value(std::int64_t idx);

    std::string to_string_as_symbol_vector() const;
    std::string to_string_as_number_vector() const;

    inline void validation_symbol(const char* error_text) const {
        if (_type != lexpr::lvector_type::symbol)
            throw std::invalid_argument(error_text);
    };
    inline void validation_number(const char* error_text) const {
        if (_type != lexpr::lvector_type::number)
            throw std::invalid_argument(error_text);
    }

    inline static void validation_symbol(const lvector& left, const lvector& right,
                                       const char* error_text) {
        validation_type(lexpr::lvector_type::symbol, left, right, error_text);
    }

    inline static void validation_number(const lvector& left, const lvector& right,
                                       const char* error_text) {
        validation_type(lexpr::lvector_type::number, left, right, error_text);
    }

    inline static void validation_type(lexpr::lvector_type type, const lvector& left, const lvector& right,
                                       const char* error_text) {
        if (left._type != right._type || left._type != type)
            throw std::invalid_argument(error_text);
    }

    inline static void validation_system_size(const lvector& left, const lvector& right,
                                              const char* error_text) {
        if (left._data.size() != right._data.size())
            throw std::invalid_argument(error_text);
    }

    std::vector< lvector_value > _data;
    lvector_type _type;
};

}

