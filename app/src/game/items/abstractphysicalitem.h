#pragma once

#include "item/drawable.h"

#include "util/enumflag.h"
#include "util/geometryoperation.h"

struct b2Body;

namespace Game
{

template <typename StateType> class AbstractPhysicalItem : public Graphics::Drawable
{
public:
    using BaseType = AbstractPhysicalItem<StateType>;

    struct Context
    {
        float velocity{ 0.f };
        float jumpImpulse{ 0.f };
    };

    AbstractPhysicalItem(b2Body *collider, const Context &context);
    void updateState(StateType state, bool isActive);
    bool isStateActive(StateType state) const;
    sf::FloatRect boundingRect() const;

protected:
    b2Body *collider();
    const Context &context() const;

private:
    b2Body *_collider{ nullptr };
    const Context _context;
    Util::EnumFlag<StateType> _state;
};

template <typename StateType>
AbstractPhysicalItem<StateType>::AbstractPhysicalItem(b2Body *collider, const Context &context)
    : _collider{ collider }, _context{ context }
{
}

template <typename StateType>
void AbstractPhysicalItem<StateType>::updateState(StateType state, bool isActive)
{
    isActive ? _state.set(state) : _state.unset(state);
}

template <typename StateType>
bool AbstractPhysicalItem<StateType>::isStateActive(StateType state) const
{
    return _state.test(state);
}

template <typename StateType> sf::FloatRect AbstractPhysicalItem<StateType>::boundingRect() const
{
    return Util::convertBodyToSFMLShape(_collider).getGlobalBounds();
}

template <typename StateType> b2Body *AbstractPhysicalItem<StateType>::collider()
{
    return _collider;
}

template <typename StateType>
const typename AbstractPhysicalItem<StateType>::Context &
AbstractPhysicalItem<StateType>::context() const
{
    return _context;
}

} // namespace Game
