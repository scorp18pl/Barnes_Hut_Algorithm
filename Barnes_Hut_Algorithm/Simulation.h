#pragma once

#include "SFML/Graphics.hpp"

#include <algorithm>
#include <vector>
#include "MyRandom.h"
#include "Map.h"
#include "Entity.h"
#include "Node.h"

class Simulation {
private:
	Map map;

	const size_t ENTITY_COUNT = 32;
	bool draw_tree;
	std::vector<CircEntity *> entities;

	QuadTree *quad_tree;

	sf::RenderWindow* window;

	void generateEntities();
	//sf::Vector2f getSmallCoordinates();
	//sf::Vector2f getBigCoordinates();

	void moveView(sf::Vector2f v);
	void zoomView(float delta);

	void toggleTree();

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

