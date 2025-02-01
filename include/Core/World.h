#pragma once

#include <cstdint>

#include "LevelManager.h"
#include "SoundManager.h"
#include "Gameplay/Ball.h"
#include "Gameplay/Paddle.h"
#include "Gameplay/Powerups.h"
#include "SFML/Window/Event.hpp"


namespace sf
{
    class RenderWindow;
}

namespace tmx
{
    class Map;
}

class MapLayer;
class ObjectLayer;

class World
{
public:
    ~World();

    // TO-DO: Ideally the scene should be read from file.
    bool load();
    void handleCollisionsWithBricks();
    void handleCollisionsWithUnbeakableBricks();
    void handleCollisionsWithPaddle();
    void checkBallBoundaries();
    void handlePowerUpCollisions();
    void increasePaddleSize();
    void activatePowerUp(PowerUps::Type type);
    void createExtraBalls();
    void setGameStateGameOver(sf::RenderWindow& window);
    void setGameStateVictory(sf::RenderWindow& window);
    void update(uint32_t deltaMilliseconds);
    void render(sf::RenderWindow& window);
    bool unload();
    void reset();
    void setWindow(sf::RenderWindow* render_window);

private:
    // This is just an example. Think a good way to group the actors of your game. If they need any type of manager, etc...
    std::vector<PowerUps*> m_powerUps;
    std::vector<Ball*> m_balls;
    Paddle* m_paddle{nullptr};
    bool m_isGameOver{false};
    bool m_isVictory{false};

    // To-Do: This should be in its own class, something like "Level" should work
    tmx::Map* m_map{nullptr};
    MapLayer* m_layerZero{nullptr};
    ObjectLayer* m_collisionLayer{nullptr};
    ObjectLayer* m_unbreakableLayer{nullptr};

    SoundManager m_soundManager;
    LevelManager m_levelManager;
    sf::Texture m_replayButtonTexture;
    sf::Sprite m_replayButtonSprite;
    bool m_shouldReset = false;
    sf::RenderWindow* m_window{nullptr};
};
