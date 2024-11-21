#pragma once

#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "resources/resourceManager.h"

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
    void animation(float deltatime);
    void updatePosition(float deltatime);

private:
    const float _speed = 200.f;

    std::vector<sf::Texture> &_walkTextures
        = ResourseManager::getInstance()->getTextures(TextureType::Player_walk);
    std::vector<sf::Texture> &_jumpTextures
        = ResourseManager::getInstance()->getTextures(TextureType::Player_jump);
    std::vector<sf::Texture> &_runTextures
        = ResourseManager::getInstance()->getTextures(TextureType::Player_run);

    PointF _position{300,300};
    sf::Sprite _sprite;

    int _currentFrame{ 0 };
    float _frameTime{ 0.1f };
    float _elapsedTime{ 0.f };

    bool _runMode = { false };
    bool _slideMode = { false };

    bool _movingRight{ false };
    bool _movingLeft = { false };
};
