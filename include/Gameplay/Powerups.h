#pragma once
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/System/Vector2.hpp"

/**
 * @brief this class represents the powerups that can be collected by the player
 */
class PowerUps
{
public:
    enum class Type
    {
        ExtraBalls,
        BiggerPaddle
    };

    struct PowerUpsDescriptor
    {
        sf::Vector2f position;
        sf::Texture* texture{nullptr};
        Type type;
    };

    PowerUps() = default;
    ~PowerUps() = default;

    bool init(const PowerUpsDescriptor& descriptor);
    void update(float deltaMilliseconds);
    void render(sf::RenderWindow& window);
    sf::FloatRect getBounds() const;
    Type getType() const;

private:
    sf::Sprite m_sprite;
    Type m_type;
    float m_speed{100.f}; // Speed at which the powerup falls
};