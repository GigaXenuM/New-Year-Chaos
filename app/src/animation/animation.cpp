#include "animation.h"

Animation::Animation(std::vector<sf::Texture> &textures, float frameTime)
    : _textures(textures), _frameTime(frameTime)
{
}

void Animation::start()
{
    _isPlaying = true;
    _currentFrame = 0;
    _elapsedTime = 0.f;
}

void Animation::stop()
{
    _isPlaying = false;
    _currentFrame = 0;
}

void Animation::update(float deltatime, sf::Sprite &sprite)
{
    if (!_isPlaying || _textures.empty())
        return;

    _elapsedTime += deltatime;
    if (_elapsedTime >= _frameTime)
    {
        _elapsedTime = 0.f;
        _currentFrame = (_currentFrame + 1) % _textures.size();
        sprite.setTexture(_textures[_currentFrame]);
    }
}