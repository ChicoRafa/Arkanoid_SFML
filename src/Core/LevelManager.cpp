#include "Core/LevelManager.h"

#include "Core/AssetManager.h"
#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Text.hpp"
#include "Utils/Constants.h"

LevelManager::LevelManager()
{
    m_font.loadFromFile(TEXT_FONT);
}

void LevelManager::setGameStateGameOver(sf::RenderWindow& window)
{
    sf::Text gameOverText;
    gameOverText.setFont(m_font);
    gameOverText.setString("GAME OVER");
    gameOverText.setCharacterSize(50);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setStyle(sf::Text::Bold);
    gameOverText.setPosition(SCREEN_WIDTH / 2.f - gameOverText.getGlobalBounds().width / 2.f, SCREEN_HEIGHT / 2.f - gameOverText.getGlobalBounds().height / 2.f);
    window.draw(gameOverText);
}

void LevelManager::setGameStateVictory(sf::RenderWindow& window)
{
    sf::Text victoryText;
    victoryText.setFont(m_font);
    victoryText.setString("VICTORY");
    victoryText.setCharacterSize(50);
    victoryText.setFillColor(sf::Color::Green);
    victoryText.setStyle(sf::Text::Bold);
    victoryText.setPosition(SCREEN_WIDTH / 2.f - victoryText.getGlobalBounds().width / 2.f, SCREEN_HEIGHT / 2.f - victoryText.getGlobalBounds().height / 2.f);
    window.draw(victoryText);
}