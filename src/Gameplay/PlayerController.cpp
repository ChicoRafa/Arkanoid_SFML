#include "Gameplay/PlayerController.h"
#include "SFML/Graphics/RenderWindow.hpp"
#include "Utils/Constants.h"
/**
 * @brief This class contains the blueprint for the player.
 */
bool PlayerController::init(const PlayerControllerDescriptor& playerControllerDescriptor)
{
    m_sprite.setTexture(*playerControllerDescriptor.texture);
    m_sprite.setPosition(playerControllerDescriptor.position);
    m_tileWidth = playerControllerDescriptor.tileWidth;
    m_tileHeight = playerControllerDescriptor.tileHeight;
    m_collisionBox.setSize(sf::Vector2f(playerControllerDescriptor.tileWidth, playerControllerDescriptor.tileHeight));
    m_collisionBox.setFillColor(sf::Color::Transparent);
    /*Debugging purposes
     *m_collisionBox.setOutlineColor(sf::Color::Red);
    m_collisionBox.setOutlineThickness(1.0f);*/
    m_collisionBox.setPosition(playerControllerDescriptor.position);

    m_lives = 3;
    m_score = 0;

    m_font.loadFromFile(TEXT_FONT);
    m_livesText.setFont(m_font);
    m_livesText.setCharacterSize(24);
    m_livesText.setFillColor(sf::Color::White);
    updateLivesText();

    m_scoreText.setFont(m_font);
    m_scoreText.setCharacterSize(24);
    m_scoreText.setFillColor(sf::Color::White);
    m_scoreText.setPosition(10.f, 30.f);
    
    return true;
}

void PlayerController::update(float deltaMilliseconds)
{
    m_sprite.setPosition(m_position);
}

void PlayerController::render(sf::RenderWindow& window)
{
    m_sprite.setTextureRect(sf::IntRect(0, 0, m_tileWidth, m_tileHeight));
    window.draw(m_sprite);
    window.draw(m_collisionBox);
    window.draw(m_livesText);
}

sf::FloatRect PlayerController::getBounds() const
{
    return m_collisionBox.getGlobalBounds();
}

void PlayerController::setLives(int lives)
{
    m_lives = lives;
    updateLivesText();
}

int PlayerController::getLives() const
{
    return m_lives;
}

void PlayerController::updateLivesText()
{
    m_livesText.setString("Lives: " + std::to_string(m_lives));
    m_livesText.setPosition(10.f, 10.f);
   // m_soundManager.playSound("life_lost");
}

void PlayerController::addScore(int points)
{
    switch (m_lives)
    {
    case 3:
        points *= 2;
        break;
    case 2:
        points = static_cast<int>(points * 1.5);
        break;
    case 1: break;
    }
    m_score += points;
    m_scoreText.setString("Score: " + std::to_string(m_score));
}

void PlayerController::renderScore(sf::RenderWindow& window)
{
    window.draw(m_scoreText);
}

