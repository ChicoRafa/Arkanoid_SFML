#include "Core/SoundManager.h"

/**
 * @brief This class is in charge of managing the sounds of the game.
 */
bool SoundManager::loadSound(const std::string& soundToLoad, const std::string& filename)
{
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile(filename))
    {
        return false;
    }
    m_soundBuffers[soundToLoad] = buffer;
    m_sounds[soundToLoad].setBuffer(m_soundBuffers[soundToLoad]);
    return true;
}

bool SoundManager::loadAllSounds()
{
    return loadSound("paddle", "../Data/Sounds/Arkanoid SFX-Paddle.wav") &&
        loadSound("brick", "../Data/Sounds/Arkanoid SFX-Brick.wav") &&
        loadSound("unbreakable", "../Data/Sounds/Arkanoid SFX-Unbreakable.wav") &&
        loadSound("powerup", "../Data/Sounds/Arkanoid SFX-Powerup.wav") &&
        loadSound("increase", "../Data/Sounds/Arkanoid SFX-Increase.wav") &&
        loadSound("life_lost", "../Data/Sounds/Arkanoid SFX-LifeLost.wav") &&
        loadSound("victory", "../Data/Sounds/Arkanoid SFX-Victory.wav") &&
        loadSound("game_over", "../Data/Sounds/Arkanoid SFX-GameOver.wav");
}

void SoundManager::playSound(const std::string& soundToPlay)
{
    if (m_sounds.find(soundToPlay) != m_sounds.end())
    {
        if (m_sounds[soundToPlay].getStatus() == sf::Sound::Stopped)
        {
            m_sounds[soundToPlay].setVolume(m_volume);
            m_sounds[soundToPlay].play();
        }
    }
}

void SoundManager::stopSound(const std::string& soundToStop)
{
    if (m_sounds.find(soundToStop) != m_sounds.end())
    {
        m_sounds[soundToStop].stop();
    }
}

void SoundManager::setVolume(float volume)
{
    m_volume = volume;
    for (auto& sound : m_sounds)
    {
        sound.second.setVolume(m_volume);
    }
}