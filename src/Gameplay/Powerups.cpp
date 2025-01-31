#include "Gameplay/Powerups.h"
#include "SFML/Graphics/RenderWindow.hpp"
/**
 * @brief this class represents the powerups that can be collected by the player
 */
bool PowerUps::init(const PowerUpsDescriptor& descriptor)
{
    m_sprite.setTexture(*descriptor.texture);
    m_sprite.setPosition(descriptor.position);
    m_sprite.setTextureRect(sf::IntRect(0, 0, 128, 128)); // Define the texture rectangle
    m_sprite.setScale(24.f / 128.f, 24.f / 128.f); // Scale down to 24x24
    m_type = descriptor.type;

    return true;
}

void PowerUps::update(float deltaMilliseconds)
{
    m_sprite.move(0.f, m_speed * deltaMilliseconds / 1000.f);
}

void PowerUps::render(sf::RenderWindow& window)
{
    window.draw(m_sprite);
}

sf::FloatRect PowerUps::getBounds() const
{
    return m_sprite.getGlobalBounds();
}

PowerUps::Type PowerUps::getType() const
{
    return m_type;
}