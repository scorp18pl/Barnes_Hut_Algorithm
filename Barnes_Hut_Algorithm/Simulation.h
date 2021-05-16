#pragma once

#include "SFML/Graphics.hpp"

#include <algorithm>
#include "MyRandom.h"
#include "Map.h"
#include "Entity.h"
#include "Node.h"

class Simulation {
private:
	Map map;

	const size_t ENTITY_COUNT = 32;
	CircGravEntity *entities;

	QuadTree *quad_tree;

	sf::RenderWindow* window;

	void generateEntities();
	//sf::Vector2f getSmallCoordinates();
	//sf::Vector2f getBigCoordinates();

	void pollEvents();
	void update();
	void render();
public:

	//Constructors and destructors
	Simulation(size_t entity_count);
	~Simulation();

	void start();
	bool isRunning();
};

