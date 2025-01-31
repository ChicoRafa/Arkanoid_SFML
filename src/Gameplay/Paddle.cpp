#include "Gameplay/Paddle.h"
#include "SFML/Window/Keyboard.hpp"
#include "Utils/Constants.h"
/** 
 * @brief This Paddle class is a PlayerController that represents the paddle in the game
 * and handles particularities.
 */
bool Paddle::init(const PaddleDescriptor& playerDescriptor)
{
    m_speed = playerDescriptor.speed;
    m_position = playerDescriptor.position;
    m_sprite.setPosition(m_position);
    m_originalScale = m_sprite.getScale();

    m_collisionBox.setSize(sf::Vector2f(playerDescriptor.tileWidth, playerDescriptor.tileHeight));
    m_collisionBox.setFillColor(sf::Color::Transparent);
    m_collisionBox.setOutlineColor(sf::Color::Red);
    m_collisionBox.setOutlineThickness(1.0f);
    m_collisionBox.setPosition(playerDescriptor.position);

    return PlayerController::init(playerDescriptor);
}

void Paddle::update(float deltaMilliseconds)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        m_direction.x = -1.0f;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        m_direction.x = 1.0f;
    }
    else
    {
        m_direction.x = .0f;
    }

    m_position.x += (m_direction.x * m_speed.x * deltaMilliseconds);
    m_position.y += (m_direction.y * m_speed.y * deltaMilliseconds);

    float halfWidth = m_tileWidth / 2.0f;
    m_position.x = std::clamp(m_position.x, -halfWidth, SCREEN_WIDTH - halfWidth);


    m_sprite.setPosition(m_position);
    m_collisionBox.setPosition(m_position);

    updateSizePowerUpTimer(deltaMilliseconds);

    PlayerController::update(deltaMilliseconds);
}

sf::Vector2f Paddle::getSize() const
{
    return sf::Vector2f(m_tileWidth, m_tileHeight);
}

void Paddle::setSize(float width, float height)
{
    m_tileWidth = width;
    m_tileHeight = height;
    m_collisionBox.setSize(sf::Vector2f(width, height));
    //mirar
    
    m_sprite.setScale(width / m_sprite.getLocalBounds().width, height / m_sprite.getLocalBounds().height);
}

void Paddle::startPowerUpTimer()
{
    m_isPowerUpActive = true;
    m_powerUpClock.restart();
}

void Paddle::updateSizePowerUpTimer(float deltaMilliseconds)
{
    if (m_powerUpClock.getElapsedTime().asSeconds() > 10.f && m_isPowerUpActive)
    {
        setSize(getSize().x / SIZE_MULTIPLIER, getSize().y / SIZE_MULTIPLIER);
        m_sprite.setScale(m_originalScale);
        m_isPaddleSizeIncreased = false;
        m_isPowerUpActive = false;
    }
}
