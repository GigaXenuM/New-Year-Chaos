#pragma once

#include <type_traits>

namespace Util
{

template <typename T> class EnumFlag
{
    static_assert(std::is_enum<T>::value, "T must be an enum type");

public:
    template <typename... Args> EnumFlag(Args... values)
    {
        fillData(values...);
    }
    EnumFlag() = default;

    void set(T value)
    {
        _data |= (1ULL << static_cast<size_t>(value));
    }

    void unset(T value)
    {
        _data &= ~(1ULL << static_cast<size_t>(value));
    }

    bool test(T value) const
    {
        return (_data & (1ULL << static_cast<size_t>(value))) != 0;
    }

private:
    void fillData(T value)
    {
        set(value);
    }
    template <typename... Args> void fillData(T value, Args... args)
    {
        set(value);
        fillData(args...);
    }

    size_t _data{ 0 };
};

} // namespace Util
