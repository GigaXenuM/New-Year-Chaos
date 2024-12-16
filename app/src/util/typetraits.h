#pragma once

#include <type_traits>

namespace Util
{

template <typename T, typename = void> struct IsPrefixedIncremental : std::false_type
{
};

template <typename T>
struct IsPrefixedIncremental<T, std::void_t<decltype(++std::declval<T &>())>> : std::true_type
{
};

template <typename T, typename = void> struct IsPostfixedIncremental : std::false_type
{
};

template <typename T>
struct IsPostfixedIncremental<T, std::void_t<decltype(std::declval<T &>()++)>> : std::true_type
{
};

template <typename T, typename = void> struct IsLessComparable : std::false_type
{
};

template <typename T>
struct IsLessComparable<T, std::void_t<decltype(std::declval<T>() < std::declval<T>())>>
    : std::true_type
{
};

template <typename T, typename = void> struct IsGraterComparable : std::false_type
{
};

template <typename T>
struct IsGraterComparable<T, std::void_t<decltype(std::declval<T>() > std::declval<T>())>>
    : std::true_type
{
};

template <typename T> constexpr bool IS_PREFIXED_INCREMENTAL_V = IsPrefixedIncremental<T>::value;
template <typename T> constexpr bool IS_POSTFIXED_INCREMENTAL_V = IsPostfixedIncremental<T>::value;
template <typename T> constexpr bool IS_LESS_COMPARABLE_V = IsLessComparable<T>::value;
template <typename T> constexpr bool IS_GRATER_COMPARABLE_V = IsGraterComparable<T>::value;

} // namespace Util
