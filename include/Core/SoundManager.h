#pragma once
#include <SFML/Audio.hpp>
#include <unordered_map>
#include <string>
/**
 * @brief This class is in charge of managing the sounds of the game.
 */
class SoundManager
{
public:
    bool loadAllSounds();
    void playSound(const std::string& soundToPlay);
    void stopSound(const std::string& soundToStop);
    void setVolume(float volume);

private:
    bool loadSound(const std::string& soundToLoad, const std::string& filename);
    std::unordered_map<std::string, sf::SoundBuffer> m_soundBuffers;
    std::unordered_map<std::string, sf::Sound> m_sounds;
    float m_volume{100.0f};
};

