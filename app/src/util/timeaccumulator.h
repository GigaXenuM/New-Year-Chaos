#pragma once

#include "iupdatable.h"

namespace Util
{

class TimeAccumulator : public IUpdatable
{
public:
    void update(float deltatime);
    bool resolve(float timeStamp);

private:
    float _data{ 0 };
};

} // namespace Util
