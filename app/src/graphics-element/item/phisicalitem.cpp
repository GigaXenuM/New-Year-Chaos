#include "phisicalitem.h"

namespace
{

constexpr float EPSILON = 1e-6f;
constexpr float MIN_OVERLAP = 0.2f;

float crossProduct(const sf::Vector2f &v1, const sf::Vector2f &v2)
{
    return v1.x * v2.y - v1.y * v2.x;
}

// Helper function: returns true if two floats are approximately equal
bool areAlmostEqual(float a, float b, float epsilon = EPSILON)
{
    return std::abs(a - b) < epsilon;
}

// Helper function: gets transformed points of a shape
std::vector<sf::Vector2f> getTransformedPoints(const sf::Shape &shape)
{
    size_t pointCount = shape.getPointCount();
    std::vector<sf::Vector2f> points;
    points.reserve(pointCount);

    for (size_t i = 0; i < pointCount; ++i)
    {
        points.push_back(shape.getTransform().transformPoint(shape.getPoint(i)));
    }

    return points;
}

// Helper function: checks if two line segments intersect and returns intersection point
bool doSegmentsIntersect(const sf::Vector2f &A, const sf::Vector2f &B, const sf::Vector2f &C,
                         const sf::Vector2f &D, sf::Vector2f &intersection)
{
    sf::Vector2f AB = B - A;
    sf::Vector2f CD = D - C;
    sf::Vector2f AC = C - A;

    float denom = crossProduct(AB, CD);
    if (areAlmostEqual(denom, 0.f))
        return false; // Parallel lines

    float t = crossProduct(AC, CD) / denom;
    float u = crossProduct(AC, AB) / denom;

    if (t >= 0.f && t <= 1.f && u >= 0.f && u <= 1.f)
    {
        intersection = A + t * AB;
        return true;
    }

    return false;
}

// Collision detection and resolution between rectangle and convex shape
std::pair<bool, sf::Vector2f> resolveCollision(const sf::RectangleShape &player,
                                               const sf::Shape &terrain, float speed)
{
    sf::FloatRect bounds = player.getGlobalBounds();

    // Define rectangle corners
    const std::vector<sf::Vector2f> rectPoints
        = { { bounds.left, bounds.top },
            { bounds.left + bounds.width, bounds.top },
            { bounds.left, bounds.top + bounds.height },
            { bounds.left + bounds.width, bounds.top + bounds.height } };

    // Get terrain points
    const std::vector<sf::Vector2f> terrainPoints = getTransformedPoints(terrain);

    bool collisionDetected = false;
    sf::Vector2f correction{ 0.f, 0.f };

    // Check each rectangle edge against each terrain edge
    for (size_t i = 0; i < rectPoints.size(); ++i)
    {
        const sf::Vector2f &rectStart = rectPoints[i];
        const sf::Vector2f &rectEnd = rectPoints[(i + 1) % rectPoints.size()];

        for (size_t j = 0; j < terrainPoints.size(); ++j)
        {
            const sf::Vector2f &terrainStart = terrainPoints[j];
            const sf::Vector2f &terrainEnd = terrainPoints[(j + 1) % terrainPoints.size()];

            sf::Vector2f intersection;
            if (doSegmentsIntersect(rectStart, rectEnd, terrainStart, terrainEnd, intersection))
            {
                collisionDetected = true;

                // Calculate the vertical displacement
                float verticalDisplacement = (bounds.top + bounds.height) - intersection.y;

                // Only adjust correction if significant overlap exists
                if (verticalDisplacement > MIN_OVERLAP)
                    correction.y = std::min(correction.y, -verticalDisplacement - speed);
            }
        }
    }

    return { collisionDetected, correction };
}

} // namespace

namespace Graphics
{

PhisicalItem::PhisicalItem(sf::RectangleShape *collider, const PhisicalContext &context,
                           EventHandler *parent)
    : AbstractItem{ parent }, _collider{ collider }, _context{ context }
{
}

void PhisicalItem::setPosition(const sf::Vector2f &position)
{
    _collider->setPosition(position);
}

sf::Vector2f PhisicalItem::position() const
{
    return _collider->getPosition();
}

sf::FloatRect PhisicalItem::globalRect() const
{
    return _collider->getGlobalBounds();
}

sf::FloatRect PhisicalItem::localRect() const
{
    return _collider->getLocalBounds();
}

void PhisicalItem::handleCollision(const sf::Shape &externalCollider)
{
    const auto &[onGround, correction]
        = resolveCollision(*_collider, externalCollider, _context.speed);
    onGround ? _state.set(State::OnGround) : _state.unset(State::OnGround);
    _collisionCorrection = correction;
}

void PhisicalItem::updateState(State state, bool isActive)
{
    isActive ? _state.set(state) : _state.unset(state);
}

bool PhisicalItem::isStateActive(State state) const
{
    return _state.test(state);
}

void PhisicalItem::updatePosition(float deltatime)
{
    sf::Vector2f correction = _collisionCorrection;

    if (isStateActive(State::Right))
        correction.x += _context.speed;
    if (isStateActive(State::Left))
        correction.x -= _context.speed;

    correction += gravityCorrection();
    handleMoving(correction, deltatime);
}

void PhisicalItem::update(float deltatime)
{
    updatePosition(deltatime);
}

sf::Vector2f PhisicalItem::gravityCorrection()
{
    const bool onGround{ isStateActive(State::OnGround) };

    _verticalVelocity = onGround ? 0.f : _verticalVelocity + _context.gravity;

    const float X{ 0.f };
    const float Y{ _verticalVelocity };

    return { X, Y };
}

void PhisicalItem::handleMoving(const sf::Vector2f &delta, float deltatime)
{
    const float magnitude{ static_cast<float>(sqrt(delta.x * delta.x + delta.y * delta.y)) };
    if (!(magnitude > 0))
        return;

    const sf::Vector2f normalizedVelocity{ delta / magnitude };
    _collider->move(normalizedVelocity * _context.speed * deltatime);
}

} // namespace Graphics
