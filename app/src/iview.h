#pragma once

#include "eventhandler.h"
#include "iupdatable.h"

#include <SFML/Graphics/RenderTarget.hpp>

namespace sf
{
class View;
}

enum class MenuType
{
    Default,
    GameOver,
    Victory,
};

class IView : public IUpdatable, public EventHandler
{
public:
    IView(sf::RenderTarget *renderTarget, EventHandler *parent)
        : EventHandler{ parent }, _renderTarget{ renderTarget }
    {
    }

    virtual void updateMenuLayout(const MenuType type)
    {
    }

    void update(float deltatime) override
    {
        _renderTarget->setView(*view());
    }

    virtual sf::View *view() const = 0;

    virtual void updateViewSize(const sf::Vector2f &size) = 0;

private:
    sf::RenderTarget *_renderTarget{ nullptr };
};
