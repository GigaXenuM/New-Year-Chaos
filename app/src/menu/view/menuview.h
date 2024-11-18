#pragma once

#include "event/eventhandler.h"
#include "iview.h"

#include <SFML/Graphics/View.hpp>

template <typename T> struct Rect;

namespace sf
{
class RenderTarget;
} // namespace sf

class MenuView : public IView, public EventHandler, public sf::View
{
    using RectF = Rect<float>;

public:
    explicit MenuView(sf::RenderTarget *renderTarget, RectF rect, EventHandler *parent);

    void update(float deltaTime) override;

private:
    sf::RenderTarget *_renderTarget;
};
