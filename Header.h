#pragma once
#include <SFML/Graphics.hpp>
class Practice {
public:
	void Update(sf::RenderWindow& window);
	void Enter();
	void Bullet(sf::RenderWindow& window,sf::Sprite& player,bool isBulletRelease,sf::Event& event);
};