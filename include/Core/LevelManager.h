#pragma once
#include "SoundManager.h"
#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/RenderWindow.hpp"


class LevelManager
{
public:
    LevelManager();
    void setGameStateGameOver(sf::RenderWindow& window);
    void setGameStateVictory(sf::RenderWindow& window);

private:
    sf::Font m_font;
};
