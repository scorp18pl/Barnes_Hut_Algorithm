#include "Map.h"

bool Map::isInside(sf::Vector2f position) {
	bool inside = position.x > this->starting_pos.x && position.x < this->ending_pos.x;
	inside = inside && position.y > this->starting_pos.y && position.y < this->ending_pos.y;
	
	return inside;
}

float Map::getSide() {
	return this->ending_pos.x - this->starting_pos.x;
}

sf::Vector2f Map::getStartingPosition() {
	return this->starting_pos;
}

sf::Vector2f Map::getEndingPosition() {
	return this->ending_pos;
}

Map::Map(float side) 
	: starting_pos(-side / 2.0f, -side / 2.0f), ending_pos(side / 2.0f, side / 2.0f) {
}

Map::Map(sf::Vector2f starting_pos, sf::Vector2f ending_pos)
	:starting_pos(starting_pos), ending_pos(ending_pos) {
}
