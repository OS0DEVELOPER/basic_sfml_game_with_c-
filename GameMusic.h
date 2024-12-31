#ifndef _GAMEMUSIC_H_
#define _GAMEMUSIC_H_

#include <SFML/Audio/Music.hpp>
#include <iostream>

class GameMusic
{
public:
    GameMusic();
    ~GameMusic();

    void playMusic();   // Method to play music
    void stopMusic();   // Method to stop music
    void setVolume(float volume);  // Method to set volume

private:
    sf::Music music;  // sf::Music object to handle audio
};

#endif // _GAMEMUSIC_H_
