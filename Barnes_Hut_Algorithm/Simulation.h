#pragma once

#include "SFML/Graphics.hpp"

#include "Entity.h"
#include "Node.h"

class Simulation
{
private:
	const size_t ENTITY_COUNT = 32;
	GravEntity *entities;

	QuadTree quad_tree;

	sf::RenderWindow* window;

	void update();
	void render();
public:

	//Constructors and destructors
	Simulation();
	~Simulation();

	void start();
	bool isRunning();
};

