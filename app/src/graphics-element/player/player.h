#pragma once

#include <item/abstractitem.h>

class Player : public Graphics::AbstractItem
{
public:
    Player(EventHandler *eventHandler);

protected:
    RectF globalRect() const override;
    RectF localRect() const override;
    PointF center() const override;

    void setPos(PointF position) override;
    void setOrigin(Align origin) override;

    void keyPressEvent(KeyPressEvent *event) override;
    void keyReleaseEvent(KeyReleaseEvent *event) override;

    void update(float deltatime) override;
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

private:
    sf::RenderTarget *_targetRender;
};
