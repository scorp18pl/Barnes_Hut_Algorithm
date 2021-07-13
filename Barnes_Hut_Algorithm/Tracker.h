#pragma once

#include <iostream>
#include "SFML/Graphics.hpp"

class Tracker {
private:
	static const size_t MAX_LENGTH = 256UL;
	static bool limit;
	std::vector<sf::Vertex> vertices;
public:
	void addVertex(sf::Vector2f position);
	void clear();
	void draw(sf::RenderWindow *window);

	static void toggleLimit();

	Tracker();
	~Tracker();
};

