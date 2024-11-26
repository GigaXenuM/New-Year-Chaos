#pragma once

#include "SFML/Graphics/RenderTarget.hpp"
#include "iupdatable.h"

namespace sf
{
class View;
}

class IView : public IUpdatable
{
public:
    IView(sf::RenderTarget *renderTarget, EventHandler *parent)
        : IUpdatable{ parent }, _renderTarget{ renderTarget }
    {
    }

    void update(float deltatime)
    {
        _renderTarget->setView(*view());
    }

    virtual sf::View *view() const = 0;

private:
    sf::RenderTarget *_renderTarget{ nullptr };
};
