#include "animation.h"

Animation::Animation(std::vector<sf::Texture> &textures, float frameTime)
    : _textures(textures), _frameTime(frameTime)
{
}

void Animation::start(const float deltatime, sf::Sprite &sprite)
{
    _elapsedTime += deltatime;
    if (_elapsedTime >= _frameTime)
    {
        _elapsedTime = 0.f;
        _currentFrame = (_currentFrame + 1) % _textures.size();
        sprite.setTexture(_textures[_currentFrame]);
    }
}

void Animation::stop()
{
    _currentFrame = 0;
}