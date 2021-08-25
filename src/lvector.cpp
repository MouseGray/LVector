#include "../include/lvector.h"

#include <numeric>
#include <algorithm>
#include <functional>
#include <iostream>

bool lexpr::lvector::operator==(const lexpr::lvector &val) const noexcept
{
    return _type != val._type &&
            std::equal(_data.begin(), _data.end(), val._data.begin(), val._data.end(),
    [type = _type] (lvector_value left, lvector_value right) {
        return type == lvector_type::symbol ? left.symbol == right.symbol :
                                              left.number == right.number ;
    });
}

lexpr::lvector lexpr::lvector::operator~() const
{
    validation_symbol("Applied complement to number");
    lvector result(*this);
    std::transform(result._data.begin(), result._data.end(), result._data.begin(),
                   [](lvector_value val) -> lvector_value { return !val.symbol; });
    return result;
}

lexpr::lvector lexpr::lvector::operator&(const lexpr::lvector &val) const
{
    return lvector(*this) &= val;
}

lexpr::lvector lexpr::lvector::operator|(const lexpr::lvector &val) const
{
    return lvector(*this) |= val;
}

lexpr::lvector lexpr::lvector::operator^(const lexpr::lvector &val) const
{
    return lvector(*this) ^= val;
}

lexpr::lvector lexpr::lvector::operator-() const
{
    validation_number("Applied negative to symbol");
    lvector result(*this);
    std::transform(result._data.begin(), result._data.end(), result._data.begin(),
                   [](lvector_value val) -> lvector_value { return -val.number; });
    return result;
}

lexpr::lvector lexpr::lvector::operator-(const lexpr::lvector &val) const
{
    return lvector(*this) -= val;
}

lexpr::lvector lexpr::lvector::operator+(const lexpr::lvector &val) const
{
    return lvector(*this) += val;
}

lexpr::lvector lexpr::lvector::operator*(const lexpr::lvector &val) const
{
    return lvector(*this) *= val;
}

lexpr::lvector lexpr::lvector::operator/(const lexpr::lvector &val) const
{
    return lvector(*this) /= val;
}

lexpr::lvector &lexpr::lvector::operator&=(const lexpr::lvector &val)
{
    validation_symbol(*this, val, "Applied intersection to number");
    validation_system_size(*this, val, "Applied intersection to different vectors");

    std::transform(_data.begin(), _data.end(), val._data.begin(), _data.begin(),
    [](lvector_value left, lvector_value right) -> lvector_value
    {
        return left.symbol && right.symbol;
    });
    return *this;
}

lexpr::lvector &lexpr::lvector::operator|=(const lexpr::lvector &val)
{
    validation_symbol(*this, val, "Applied union to number");
    validation_system_size(*this, val, "Applied union to different vectors");

    std::transform(_data.begin(), _data.end(), val._data.begin(), _data.begin(),
    [](lvector_value left, lvector_value right) -> lvector_value
    {
        return left.symbol || right.symbol;
    });
    return *this;
}

lexpr::lvector &lexpr::lvector::operator^=(const lexpr::lvector &val)
{
    validation_symbol(*this, val, "Applied minus to number");
    validation_system_size(*this, val, "Applied minus to different vectors");

    std::transform(_data.begin(), _data.end(), val._data.begin(), _data.begin(),
    [](lvector_value left, lvector_value right) -> lvector_value
    {
        return left.symbol && !right.symbol;
    });
    return *this;
}

lexpr::lvector &lexpr::lvector::operator-=(const lexpr::lvector &val)
{
    validation_number(*this, val, "Applied different to symbol");
    validation_system_size(*this, val, "Applied different to different vectors");

    std::transform(_data.begin(), _data.end(), val._data.begin(), _data.begin(),
    [](lvector_value left, lvector_value right) -> lvector_value
    {
        return left.number - right.number;
    });
    return *this;
}

lexpr::lvector &lexpr::lvector::operator+=(const lexpr::lvector &val)
{
    validation_number(*this, val, "Applied addition to symbol");
    validation_system_size(*this, val, "Applied addition to different vectors");

    std::transform(_data.begin(), _data.end(), val._data.begin(), _data.begin(),
    [](lvector_value left, lvector_value right) -> lvector_value
    {
        return left.number + right.number;
    });
    return *this;
}

lexpr::lvector &lexpr::lvector::operator*=(const lexpr::lvector &val)
{
    validation_number(*this, val, "Applied multiplication to symbol");
    validation_system_size(*this, val, "Applied multiplication to different vectors");

    std::transform(_data.begin(), _data.end(), val._data.begin(), _data.begin(),
    [](lvector_value left, lvector_value right) -> lvector_value
    {
        return left.number * right.number;
    });
    return *this;
}

lexpr::lvector &lexpr::lvector::operator/=(const lexpr::lvector &val)
{
    validation_number(*this, val, "Applied division to symbol");
    validation_system_size(*this, val, "Applied division to different vectors");

    std::transform(_data.begin(), _data.end(), val._data.begin(), _data.begin(),
    [](lvector_value left, lvector_value right) -> lvector_value
    {
        return left.number / right.number;
    });
    return *this;
}

lexpr::lvector &lexpr::lvector::to_quantity()
{
    validation_symbol("Execute 'to quantity' to number");

    _type = number_t::type_value;
    std::transform(_data.begin(), _data.end(), _data.begin(),
    [](lvector_value val) -> lvector_value
    {
        return static_cast<number_t::value_type>(val.symbol);
    });
    return *this;
}

std::string lexpr::lvector::to_string_as_symbol_vector() const
{
    if (_data.empty()) return {};
    return std::accumulate(std::next(_data.begin()), _data.end(), std::to_string(_data[0].symbol),
            [](std::string res, lvector_value val) {
        return std::move(res) + "," + std::to_string(val.symbol);
    });
}

std::string lexpr::lvector::to_string_as_number_vector() const
{
    if (_data.empty()) return {};
    return std::accumulate(std::next(_data.begin()), _data.end(), std::to_string(_data[0].number),
            [](std::string res, lvector_value val) {
        return std::move(res) + "," + std::to_string(val.number);
    });
}

std::string lexpr::lvector::to_string() const
{
    return  "(" + (_type == lvector_type::symbol ?
                to_string_as_symbol_vector() :
                to_string_as_number_vector()) + ")";
}

template<>
lexpr::number_t::value_type &lexpr::lvector::get_value<lexpr::number_t>(std::int64_t idx)
{
    return _data[idx].number;
}

template<>
lexpr::symbol_t::value_type &lexpr::lvector::get_value<lexpr::symbol_t>(std::int64_t idx)
{
    return _data[idx].symbol;
}
