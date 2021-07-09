#pragma once
#include "SFML/Graphics.hpp"
#include <stdbool.h>

class Map {
private:
	sf::Vector2f starting_pos, ending_pos;
public:
	bool isInside(sf::Vector2f position);
	float getSide();
	sf::Vector2f getStartingPosition();
	sf::Vector2f getEndingPosition();

	Map(float side = 1e8f);
	Map(sf::Vector2f starting_pos, sf::Vector2f ending_pos);
};

