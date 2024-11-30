#include "animation.h"

Animation::Animation(std::vector<sf::Texture> &textures, float frameTime)
    : _textures(textures), _frameTime(frameTime)
{
}

void Animation::start(const float deltatime, sf::Sprite &sprite, bool loop)
{
    if (!_isPlaying)
    {
        _isPlaying = true;
        _finished = false;
    }

    _elapsedTime += deltatime;

    if (_elapsedTime >= _frameTime)
    {
        _elapsedTime = 0.f;

        if (_currentFrame + 1 < _textures.size())
            _currentFrame++;
        else if (loop)
            _currentFrame = 0;
        else
        {
            _currentFrame = _textures.size() - 1;
            _isPlaying = false;
            _finished = true;
        }
        sprite.setTexture(_textures[_currentFrame]);
    }
}

void Animation::stop()
{
    _isPlaying = false;
    _currentFrame = 0;
    _finished = true;
}

bool Animation::isPlaying() const
{
    return _isPlaying;
}

bool Animation::isFinished() const
{
    return _finished;
}
