#pragma once
#include "GameObject.h"
#include "Paddle.h"
#include "Core/SoundManager.h"
#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/System/Clock.hpp"

class Ball: public GameObject
{
public:
    struct BallDescriptor
    {
        sf::Vector2f position;
        sf::Vector2f speed{.0f, .0f};
        const sf::Texture* texture{nullptr};
        float tileWidth{.0f};
        float tileHeight{.0f};
        float radius{.0f};
        float mass{0.1f}; //probably should be a constant, depending if we are gonna implement different ball sizes or not
        
    };

    ~Ball() override = default;

    bool init(const BallDescriptor& ballDescriptor);

    void update(float deltaMilliseconds) override;
    void render(sf::RenderWindow& window) override;

    sf::FloatRect getBounds() const override;
    sf::Vector2f getPosition() ///Can't be const
    {
        return m_shape.getPosition();
    }
    
    bool checkCollision(const GameObject& other) const;
    bool checkCollisionWithPaddle(const Paddle& paddle);
    bool CheckBottomBoundaryTreaspassing(sf::Vector2f position, PlayerController* playerController, bool& isGameOver, size_t ballCount);
    void handleCollision();
    float getRadius();
    BallDescriptor getDescriptor() const;
    const sf::Texture* getTexture() const;
    void setTexture(const sf::Texture* texture);

protected:
    sf::Sprite m_sprite;
    float m_tileWidth{.0f};
    float m_tileHeight{.0f};
    sf::Vector2f m_speed{.0f, .0f};
    float m_mass{0.1f};
    SoundManager m_soundManager;

private:
    sf::CircleShape m_shape;
    sf::Clock m_waitClock;
    bool m_isWaiting = false;

public:
    void setSpeed(const sf::Vector2f& speed);
    sf::Vector2f getSpeed() const;
    void resetPosition(const sf::Vector2f& position);
};
