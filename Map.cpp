#include<SFML/Graphics.hpp>
#include<iostream>

void LoadMap() {
	sf::Texture texture;
	sf::Sprite mapTileSheet;
	if (!texture.loadFromFile("Asserts/BasicGreen.png")) {
		std::cout << "Problem While Loading File";
	}
}
