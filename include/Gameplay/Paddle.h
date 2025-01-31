#pragma once
#include "PlayerController.h"
#include "SFML/System/Clock.hpp"

/** 
 * @brief This Paddle class is a PlayerController that represents the paddle in the game
 * and handles particularities.
 */
class Paddle : public PlayerController
{
public:
    struct PaddleDescriptor : PlayerControllerDescriptor
    {
        sf::Vector2f speed{.0f, .0f};
    };

    bool init(const PaddleDescriptor& playerDescriptor);
    void update(float deltaMilliseconds) override;
    sf::Vector2f getSize() const;
    void setSize(float width, float height);
    bool isPaddleSizeIncreased() const { return m_isPaddleSizeIncreased; }
    void setPaddleSizeIncreased(bool increased) { m_isPaddleSizeIncreased = increased; }
    void startPowerUpTimer();
    void updateSizePowerUpTimer(float deltaMilliseconds);

private:
    sf::Vector2f m_direction{.0f, .0f};
    sf::Vector2f m_speed{.0f, .0f};
    sf::Vector2f m_originalScale;
    bool m_isPaddleSizeIncreased{false};
    sf::Clock m_powerUpClock;
    bool m_isPowerUpActive{false};
    
};
