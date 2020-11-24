/* Variant classes.
 * @file variant
 * @date 2020
 * @author Серафима Рысина
 */

#ifndef __VARIANT_HPP__
#define __VARIANT_HPP__

#include <map>
#include <memory>
#include <variant>
#include <type_traits>
#include <cstdint>
#include <deque>

#include <boost/variant.hpp>

struct number : public std::variant<int8_t, int16_t, int32_t, int64_t, uint8_t, uint16_t, uint32_t, uint64_t, float, double, long double>
{ using variant::variant; };

struct array : public std::deque<number>
{ using deque::deque; };

struct recursive_array : public std::deque<std::variant<number, std::shared_ptr<recursive_array>>>
{ using deque::deque; };

struct recursive_array2 : public std::deque<std::variant<number, boost::recursive_wrapper<recursive_array2>>>
{ using deque::deque; };

template<typename... Args> struct variant_decorator : public std::variant<Args...>
{
    using variant_decorator::variant::variant;
    
    template<typename T> T& as()
    { return const_cast<T&>(const_cast<const variant_decorator*>(this)->as<T>()); }

    template<typename T> T const& as() const
    {
        if constexpr (std::disjunction_v<std::is_same<boost::recursive_wrapper<T>, Args>...>)
            return std::get<boost::recursive_wrapper<T>>(*this).get();
        else
            return std::get<T>(*this);
    }
};

struct recursive_map : public std::map<std::string, variant_decorator<bool, int8_t, int16_t, int32_t, int64_t, uint8_t, uint16_t, uint32_t, uint64_t, float, double, long double, std::string, boost::recursive_wrapper<recursive_map>>>
{ using map::map; };

#endif // __VARIANT_HPP__
