#pragma once

#include <SFML/System/Vector2.hpp>

#include "SFML/Graphics/Sprite.hpp"

namespace sf
{
    class RenderWindow;
}
/**
 * @brief Base class for all the gameObjects used in the project.
 */
class GameObject
{
public:
    GameObject() = default;
    virtual ~GameObject() = default;

    virtual const sf::Vector2f& getPosition() const { return m_position; }
    sf::Vector2f& getPosition() { return m_position; }
    void setPosition(const sf::Vector2f& newPosition) { m_position = newPosition; }

    virtual void update(float delta) = 0;
    virtual void render(sf::RenderWindow& window) = 0;

    virtual sf::FloatRect getBounds() const { return m_sprite.getGlobalBounds(); }

    /**
     * @brief Check if a circle and a rectangle are colliding
     * Used for ball vs paddle/brick collision
     * 
     * @param circleCenter The center of the circle
     * @param circleRadius The radius of the circle
     * @param rect The rectangle
     * @return true if the circle and the rectangle are colliding
     * @return false if the circle and the rectangle are not colliding
     */
    bool checkCollisionCircleRect(const sf::Vector2f& circleCenter, float circleRadius, const sf::FloatRect& rect) const
    {
        float closestX = std::clamp(circleCenter.x, rect.left, rect.left + rect.width);
        float closestY = std::clamp(circleCenter.y, rect.top, rect.top + rect.height);

        float distanceX = circleCenter.x - closestX;
        float distanceY = circleCenter.y - closestY;

        return (distanceX * distanceX + distanceY * distanceY) < (circleRadius * circleRadius);
    }

protected:
    sf::Vector2f m_position{.0f, .0f};
    sf::Sprite m_sprite;
};
