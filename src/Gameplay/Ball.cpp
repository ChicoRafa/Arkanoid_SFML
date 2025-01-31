#include "Gameplay/Ball.h"
#include "Gameplay/Paddle.h"
#include "SFML/Graphics/RenderWindow.hpp"
#include "Utils/Constants.h"

bool Ball::init(const BallDescriptor& ballDescriptor)
{
    m_shape.setRadius(ballDescriptor.radius);
    m_shape.setOrigin(ballDescriptor.radius, ballDescriptor.radius);
    m_shape.setPosition(ballDescriptor.position);
    m_speed = ballDescriptor.speed;
    m_mass = ballDescriptor.mass;

    if (ballDescriptor.texture)
    {
        m_shape.setTexture(ballDescriptor.texture);
    }

    // Set the outline color and thickness to visualize the collision shape
    m_shape.setOutlineColor(sf::Color::Red);
    m_shape.setOutlineThickness(1.0f);
    
    return true;
}

/**
 * @brief Check if the ball has trespassed the bottom boundary of the window
 * and handle the game over condition 
 * 
 * @param position -> current ball position
 * @param playerController -> the paddle
 * @param isGameOver -> flag to check if the game is over
 * @param ballCount -> number of balls in the game
 * @return 
 */
bool Ball::CheckBottomBoundaryTreaspassing(sf::Vector2f position, PlayerController* playerController, bool& isGameOver, size_t ballCount)
{
    if (position.y + m_shape.getRadius() > SCREEN_HEIGHT)
    {
        if (ballCount > 1)
        {
            return true;
        }

        // Decrement player's lives by one
        int lives = playerController->getLives();
        playerController->setLives(std::max(lives - 1, 0));
        m_soundManager.playSound("life_lost");
        // Check if the player has no more lives
        if (playerController->getLives() <= 0)
        {
            isGameOver = true;
        }
        else
        {
            // Reset ball position if the game is not over
            resetPosition({(SCREEN_WIDTH - m_shape.getRadius() * 2) / 2.f, 300.f});
        }
        return true;
    }
    return false;
}

void Ball::update(float deltaMilliseconds)
{
    if (m_isWaiting)
    {
        if (m_waitClock.getElapsedTime().asMilliseconds() > 1000)
        {
            m_isWaiting = false;
        }
        return;
    }
    // Adding gravity to the ball
    float deltaSeconds = deltaMilliseconds / 1000.f;
    m_speed += (GRAVITY / m_mass) * deltaSeconds;

    // Updating the position of the ball
    sf::Vector2f position = m_shape.getPosition();
    position += m_speed * deltaSeconds;
    m_shape.setPosition(position);

    // Checking for collisions with the window borders
    if (position.x - m_shape.getRadius() < 0 || position.x + m_shape.getRadius() > 800) // hardcoding the window width
    {
        m_speed.x = -m_speed.x; // collision with left or right window border
        position.x = std::clamp(position.x, m_shape.getRadius(), 800.f - m_shape.getRadius());
    }
    if (position.y - m_shape.getRadius() < 0) // hardcoding the window height
    {
        m_speed.y = -m_speed.y; // Collision with top window border
        position.y = std::clamp(position.y, m_shape.getRadius(), 600.f - m_shape.getRadius());
    }
    m_shape.setPosition(position);
}

void Ball::render(sf::RenderWindow& window)
{
    window.draw(m_shape);
}

bool Ball::checkCollision(const GameObject& other) const
{
    sf::FloatRect otherBounds = other.getBounds();
    sf::Vector2f circleCenter = m_shape.getPosition();
    float circleRadius = m_shape.getRadius();

    return checkCollisionCircleRect(circleCenter, circleRadius, otherBounds);
}

void Ball::handleCollision()
{
    setSpeed({-getSpeed().x, -getSpeed().y});
}

/**
 * @brief Check if the ball has collided with the paddle and endow speed to the ball
 * 
 * @param paddle -> the paddle object
 * @return 
 */
bool Ball::checkCollisionWithPaddle(const Paddle& paddle)
{
    if (m_shape.getGlobalBounds().intersects(paddle.getBounds()))
    {
        // Calculate the point of impact on the paddle
        float paddleLeft = paddle.getBounds().left;
        float paddleWidth = paddle.getBounds().width;
        float ballCenterX = m_shape.getPosition().x;

        // Calculate the relative hit position
        float relativeHitPosition = (ballCenterX - paddleLeft) / paddleWidth;

        // Adjust the ball's speed based on the hit position
        float angle = (relativeHitPosition - 0.5f) * 2.0f;
        float speed = std::sqrt(m_speed.x * m_speed.x + m_speed.y * m_speed.y);
        m_speed.x = speed * angle;
        m_speed.y = std::abs(m_speed.y); // Always bounce upwards

        
        // Adjust the ball's position to be just above the paddle
        sf::Vector2f position = m_shape.getPosition();
        position.y = paddle.getBounds().top - m_shape.getRadius() * 2;
        m_shape.setPosition(position);

        return true;
    }
    return false;
}

void Ball::setSpeed(const sf::Vector2f& speed)
{
    m_speed = speed;
}

sf::Vector2f Ball::getSpeed() const
{
    return m_speed;
}

float Ball::getRadius()
{
   return m_shape.getRadius();
}

void Ball::resetPosition(const sf::Vector2f& position)
{
    
    m_shape.setPosition(position);
    m_speed = {0.0f, 0.0f};
    m_isWaiting = true;
    m_waitClock.restart();
}

sf::Rect<float> Ball::getBounds() const
{
    return sf::Rect<float>(m_shape.getPosition().x, m_shape.getPosition().y, m_shape.getRadius() * 2, m_shape.getRadius()  * 2);
}

Ball::BallDescriptor Ball::getDescriptor() const
{
    BallDescriptor descriptor;
    descriptor.position = m_shape.getPosition();
    descriptor.speed = m_speed;
    descriptor.texture = m_sprite.getTexture();
    descriptor.tileWidth = m_tileWidth;
    descriptor.tileHeight = m_tileHeight;
    descriptor.radius = m_shape.getRadius();
    descriptor.mass = m_mass;
    return descriptor;
}

const sf::Texture* Ball::getTexture() const
{
    return m_shape.getTexture();
}

void Ball::setTexture(const sf::Texture* texture)
{
    m_shape.setTexture(&*texture);
}

