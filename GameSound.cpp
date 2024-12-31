#include "GameMusic.h"

GameMusic::GameMusic()
{
    // Attempt to load the music file
    if (!music.openFromFile("Audio/output.ogg")) {
        std::cout << "Error Loading Audio" << std::endl;
    }
}

GameMusic::~GameMusic()
{
    // Destructor (No specific cleanup needed)
}

void GameMusic::playMusic()
{   
        music.play();  // Play the music if not already playing
        music.setLoop(true);
}

void GameMusic::stopMusic()
{
    music.stop();  // Stop the music
}

void GameMusic::setVolume(float volume)
{
    music.setVolume(volume);  // Set the volume of the music (0-100)
}
