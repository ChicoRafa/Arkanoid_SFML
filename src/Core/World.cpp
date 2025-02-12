#include <Core/AssetManager.h>
#include <Core/World.h>
#include "Gameplay/Ball.h"
#include "Gameplay/Paddle.h"
#include <Render/SFMLOrthogonalLayer.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <tmxlite/Map.hpp>

#include "Core/LevelManager.h"
#include "Utils/Constants.h"

World::~World()
{
	//gameObjects
	delete m_paddle;
	for (auto ball : m_balls)
	{
		delete ball;
	}
	for (auto powerUp : m_powerUps)
	{
		delete powerUp;
	}
	//scenario
	delete m_layerZero;
	delete m_collisionLayer;
	delete m_unbreakableLayer;
	delete m_map;
}

bool World::load()
{
    constexpr float millisecondsToSeconds = 1 / 1000.f;
	
    // Load paddle texture and initialize paddle
    sf::Texture* paddleTexture = AssetManager::getInstance()->loadTexture("../Data/Levels/Images/PNG/ArkanoidAssets/Paddles/StyleB/PaddleMid.png");
    Paddle::PaddleDescriptor paddleDescriptor;
    paddleDescriptor.texture = paddleTexture;
    paddleDescriptor.tileWidth = 160.f;
    paddleDescriptor.tileHeight = 28.f;
    paddleDescriptor.position = { (SCREEN_WIDTH - paddleDescriptor.tileWidth) / 2.f, 550.f }; // Centered at the bottom
    paddleDescriptor.speed = { 400.f * millisecondsToSeconds, .0f };
    Paddle* paddle = new Paddle();
    const bool paddleInitOk = paddle->init(paddleDescriptor);
    m_paddle = paddle;

    // Load ball texture and initialize ball
    sf::Texture* ballTexture = AssetManager::getInstance()->loadTexture("../Data/Levels/Images/PNG/ArkanoidAssets/Balls/Shiny/Ball_Red_Shiny-24x24.png");
    Ball::BallDescriptor ballDescriptor;
    ballDescriptor.texture = ballTexture;
    ballDescriptor.radius = 12.f;
    ballDescriptor.position = { (SCREEN_WIDTH - ballDescriptor.radius * 2) / 2.f, 350.f }; // Centered in the middle
    ballDescriptor.speed = { 200.f * millisecondsToSeconds, 200.f * millisecondsToSeconds };
    Ball* ball = new Ball();
    const bool ballInitOk = ball->init(ballDescriptor);
    m_balls.push_back(ball);

	//Map
    m_map = new tmx::Map();
	bool mapLoadOk = m_map->load("../Data/Levels/level1.tmx");
    m_layerZero = new MapLayer(*m_map, 0);
	
    m_collisionLayer = new ObjectLayer(*m_map, 1);
	m_unbreakableLayer = new ObjectLayer(*m_map, 2);

	m_soundManager.loadAllSounds();
	// Load replay button texture and initialize sprite
	m_replayButtonTexture.loadFromFile("../Data/Buttons/replay.png");
	m_replayButtonSprite.setTexture(m_replayButtonTexture);
	m_replayButtonSprite.setScale(0.5f, 0.5f); 
	m_replayButtonSprite.setPosition((SCREEN_WIDTH - m_replayButtonTexture.getSize().x * 0.5f) / 2.f, (SCREEN_HEIGHT - m_replayButtonTexture.getSize().y * 0.5f) / 2.f + 75.f);
	
	bool allResourcesLoaded = paddleInitOk && ballInitOk && mapLoadOk;
	return allResourcesLoaded;
}

void World::update(uint32_t deltaMilliseconds)
{
	m_soundManager.playSound("music");
	if (m_isGameOver || m_isVictory)
	{
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			sf::Vector2i mousePosition = sf::Mouse::getPosition(*m_window);
			if (m_replayButtonSprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition)))
			{
				reset();
			}
		}
	}
	else
	{
		// Update level
		m_layerZero->update(sf::milliseconds(deltaMilliseconds));

		// Update actors
		for (auto& powerUp : m_powerUps)
		{
			powerUp->update(deltaMilliseconds);
		}
		for (auto& ball : m_balls)
		{
			ball->update(deltaMilliseconds);
		}
		m_paddle->update(deltaMilliseconds);

		// Handle collisions
		handleCollisionsWithBricks();
		handleCollisionsWithUnbeakableBricks();
		handleCollisionsWithPaddle();
		handlePowerUpCollisions();

		// Check if the ball falls below the bottom boundary
		checkBallBoundaries();

		// Handling victory condition (all bricks were destroyed)
		if (m_collisionLayer->getShapes().empty() && !m_isVictory)
		{
			m_isVictory = true;
			m_soundManager.playSound("victory");
		}
	}
}

void World::render(sf::RenderWindow& window)
{
	//scenario
	window.draw(*m_layerZero);
	window.draw(*m_collisionLayer);
	window.draw(*m_unbreakableLayer);

	//gameobjects
	m_paddle->render(window);
	m_paddle->renderScore(window);
	for (auto& powerUp : m_powerUps)
	{
		powerUp->render(window);
	}
	for (auto& ball : m_balls)
	{
		ball->render(window);
	}

	//game end handling
	if (m_isGameOver)
	{
		setGameStateGameOver(window);
		window.draw(m_replayButtonSprite);
	}
	if (m_isVictory)
	{
		setGameStateVictory(window);
		window.draw(m_replayButtonSprite);
	}
}

bool World::unload()
{
	for (auto powerUp : m_powerUps)
	{
		delete powerUp;
	}
	m_powerUps.clear();

	for (auto ball : m_balls)
	{
		delete ball;
	}
	m_balls.clear();

	if (m_paddle)
	{
		delete m_paddle;
		m_paddle = nullptr;
	}

	if (m_map)
	{
		delete m_map;
		m_map = nullptr;
	}
	if (m_layerZero)
	{
		delete m_layerZero;
		m_layerZero = nullptr;
	}
	if (m_collisionLayer)
	{
		delete m_collisionLayer;
		m_collisionLayer = nullptr;
	}
	if (m_unbreakableLayer)
	{
		delete m_unbreakableLayer;
		m_unbreakableLayer = nullptr;
	}

	bool allResourcesUnloaded = m_powerUps.empty() && m_balls.empty() && !m_paddle && !m_map && !m_layerZero && !m_collisionLayer && !m_unbreakableLayer;
	return allResourcesUnloaded;
}

void World::reset()
{
	unload();
	load();
	m_isGameOver = false;
	m_isVictory = false;
	m_shouldReset = false;
}

void World::handleCollisionsWithBricks()
{
    auto& collisionShapes = m_collisionLayer->getShapes();
    std::vector<sf::RectangleShape*> shapesToRemove;

    for (auto it = collisionShapes.begin(); it != collisionShapes.end(); ++it)
    {
        sf::RectangleShape* shape = dynamic_cast<sf::RectangleShape*>(*it);
        if (shape)
        {
            for (auto& ball : m_balls)
            {
                if (shape->getGlobalBounds().intersects(ball->getBounds()))
                {
                    sf::Vector2f tilePosition = shape->getPosition();
                    m_layerZero->removeTileAt(tilePosition);
                    m_layerZero->removeTileAt({tilePosition.x + 32.f, tilePosition.y});

                    shapesToRemove.push_back(shape);

                    ball->handleCollision();
                	m_soundManager.playSound("brick");
                    m_paddle->addScore(500);

                	int randomValue = rand() % 100;
                	if (randomValue < 20)
                	{
                		PowerUps::PowerUpsDescriptor powerUpDescriptor;
                		powerUpDescriptor.position = tilePosition;
                		powerUpDescriptor.texture = AssetManager::getInstance()->loadTexture("../Data/Levels/Images/PNG/ArkanoidAssets/PowerUps/powerup.png");
                		powerUpDescriptor.type = PowerUps::Type::ExtraBalls;
                		PowerUps* powerUp = new PowerUps();
                		powerUp->init(powerUpDescriptor);
                		m_powerUps.push_back(powerUp);
                	}
                	else if (randomValue < 30)
                	{
                		PowerUps::PowerUpsDescriptor powerUpDescriptor;
                		powerUpDescriptor.position = tilePosition;
                		powerUpDescriptor.texture = AssetManager::getInstance()->loadTexture("../Data/Levels/Images/PNG/ArkanoidAssets/PowerUps/powerup.png");
                		powerUpDescriptor.type = PowerUps::Type::BiggerPaddle;
                		PowerUps* powerUp = new PowerUps();
                		powerUp->init(powerUpDescriptor);
                		m_powerUps.push_back(powerUp);
                	}
                }
            }
        }
    }

    for (auto shape : shapesToRemove)
    {
        auto it = std::find(collisionShapes.begin(), collisionShapes.end(), shape);
        if (it != collisionShapes.end())
        {
            delete *it;
            collisionShapes.erase(it);
        }
    }
}

void World::handleCollisionsWithUnbeakableBricks()
{
    auto& unbreakableShapes = m_unbreakableLayer->getShapes();
    for (auto& shape : unbreakableShapes)
    {
        for (auto& ball : m_balls)
        {
            if (shape && shape->getGlobalBounds().intersects(ball->getBounds()))
            {
                ball->handleCollision();
            	m_soundManager.playSound("unbreakable");
            }
        }
    }
}

void World::handleCollisionsWithPaddle()
{
    for (auto& ball : m_balls)
    {
        if (ball->checkCollisionWithPaddle(*m_paddle))
        {
            ball->handleCollision();
        	m_soundManager.playSound("paddle");
        }
    }
}

void World::checkBallBoundaries()
{
	for (auto it = m_balls.begin(); it != m_balls.end();)
	{
		bool isOutOfBounds = (*it)->CheckBottomBoundaryTreaspassing((*it)->getPosition(), m_paddle, m_isGameOver, m_balls.size());

		if (isOutOfBounds)		{
			if (m_isGameOver)
			{
				m_soundManager.playSound("game_over");
			}
			if (m_balls.size() > 1)
			{
				delete *it;
				it = m_balls.erase(it);
			}
			else
			{
				++it;
			}
		}
		else
		{
			++it;
		}
	}
}

void World::handlePowerUpCollisions()
{
    for (auto it = m_powerUps.begin(); it != m_powerUps.end();)
    {
        if ((*it)->getBounds().intersects(m_paddle->getBounds()))
        {
            activatePowerUp((*it)->getType());
            delete *it;
            it = m_powerUps.erase(it);
        	m_soundManager.playSound("powerup");
        }
        else
        {
            ++it;
        }
    }
}

void World::increasePaddleSize()
{
	m_paddle->setSize(m_paddle->getSize().x * SIZE_MULTIPLIER, m_paddle->getSize().y * SIZE_MULTIPLIER);
	m_paddle->setPaddleSizeIncreased(true);
	m_soundManager.playSound("increase");
	m_paddle->startPowerUpTimer();
}

void World::activatePowerUp(PowerUps::Type type)
{
	if (type == PowerUps::Type::ExtraBalls && m_balls.size() < 3)
	{
		createExtraBalls();
	}
	else if (type == PowerUps::Type::BiggerPaddle && !m_paddle->isPaddleSizeIncreased())
	{
		increasePaddleSize();
	}
	
}

void World::createExtraBalls()
{
	for (int i = 0; i < 2; ++i)
	{
		Ball* newBall = new Ball();
		Ball::BallDescriptor ballDescriptor = m_balls[0]->getDescriptor();
		newBall->init(ballDescriptor);
		newBall->setTexture(m_balls[0]->getTexture());

		if (i == 0)
		{
			newBall->setSpeed({-std::abs(ballDescriptor.speed.x), ballDescriptor.speed.y});
		}
		else
		{
			newBall->setSpeed({std::abs(ballDescriptor.speed.x), -ballDescriptor.speed.y});
		}

		m_balls.push_back(newBall);
	}
}

void World::setGameStateGameOver(sf::RenderWindow& window)
{
	m_levelManager.setGameStateGameOver(window);

}

void World::setGameStateVictory(sf::RenderWindow& window)
{
	m_levelManager.setGameStateVictory(window);

}
void World::setWindow(sf::RenderWindow* window)
{
	m_window = window;
}

