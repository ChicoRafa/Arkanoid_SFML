#pragma once
#include "GameObject.h"
#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/Graphics/Texture.hpp"
/**
 * @brief This class contains the blueprint for the player.
 */
class PlayerController : public GameObject
{
public:
    struct PlayerControllerDescriptor
    {
        sf::Vector2f position;
        sf::Texture* texture{nullptr};
        float tileWidth{.0f};
        float tileHeight{.0f};
    };
    
    sf::FloatRect getBounds() const override;

    ~PlayerController() override = default;

    bool init(const PlayerControllerDescriptor& playerControllerDescriptor);

    void update(float deltaMilliseconds) override;
    void render(sf::RenderWindow& window) override;

    void setLives(int lives);
    int getLives() const;
    void updateLivesText();

    void addScore(int points);
    void renderScore(sf::RenderWindow& window);

protected:
    sf::Sprite m_sprite;
    float m_tileWidth{.0f};
    float m_tileHeight{.0f};
    sf::RectangleShape m_collisionBox;

private:
    int m_lives;
    int m_score;
    sf::Text m_livesText;
    sf::Font m_font;
    sf::Text m_scoreText;
};
