#pragma once

#include "util/enumflag.h"
#include "util/typetraits.h"

#include <algorithm>

namespace Util
{

template <typename T> class LimitedValue
{
    static_assert(Util::IS_LESS_COMPARABLE_V<T>,
                  "Expected that the type \'T\' has defined the operator \"<\".");
    static_assert(Util::IS_GRATER_COMPARABLE_V<T>,
                  "Expected that the type \'T\' has defined the operator \">\".");

public:
    enum class State
    {
        MinReached,
        MaxReached,
    };

    LimitedValue()
    {
    }
    explicit LimitedValue(T value, T min, T max) : _value{ value }, _min{ min }, _max{ max }
    {
        assert(min <= value && value <= max);
    }

    bool isMin() const
    {
        return _state.test(State::MinReached);
    }
    bool isMax() const
    {
        return _state.test(State::MaxReached);
    }

    void setValue(T value)
    {
        _state = {};

        if (value <= _min)
        {
            _value = _min;
            _state.set(State::MinReached);
        }
        if (value >= _max)
        {
            _value = _max;
            _state.set(State::MaxReached);
        }

        _value = std::max(std::min(value, _max), _min);
    }
    void move(T offset)
    {
        setValue(get() + offset);
    }
    void setMax()
    {
        setValue(_max);
    }
    void setMin()
    {
        setValue(_min);
    }
    void changeLimits(T min, T max)
    {
        _min = min;
        _max = max;
        setValue(_value);
    }

    T get() const
    {
        return _value;
    }
    T min()
    {
        return _min;
    }
    T max()
    {
        return _max;
    }

private:
    T _value{ T{} };
    T _min{ T{} };
    T _max{ T{} };
    EnumFlag<State> _state;
};

using LimitedValueF = LimitedValue<float>;

} // namespace Util
