#pragma once

#include <box2d/box2d.h>

namespace sf
{
class RenderTarget;
template <typename T> class Vector2;
using Vector2f = Vector2<float>;
class Color;
} // namespace sf

namespace Util
{

class DebugDrawer : public b2Draw
{
public:
    DebugDrawer(sf::RenderTarget *target);

    void DrawPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color);
    void DrawSolidPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color);
    void DrawCircle(const b2Vec2 &center, float radius, const b2Color &color);
    void DrawSolidCircle(const b2Vec2 &center, float radius, const b2Vec2 &axis,
                         const b2Color &color);
    void DrawSegment(const b2Vec2 &p1, const b2Vec2 &p2, const b2Color &color);
    void DrawTransform(const b2Transform &xf);
    void DrawPoint(const b2Vec2 &p, float size, const b2Color &color);

private:
    sf::RenderTarget *_target;
};

} // namespace Util
