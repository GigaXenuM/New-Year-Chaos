#include "timeaccumulator.h"

namespace
{
constexpr float DELTA_MULTIPLIER = 30.0f;
}

namespace Util
{

void TimeAccumulator::update(float deltatime)
{
    _data += deltatime * DELTA_MULTIPLIER;
}

bool TimeAccumulator::resolve(float timeStamp)
{
    if (_data < timeStamp)
        return false;

    _data -= timeStamp;
    return true;
}

} // namespace Util
