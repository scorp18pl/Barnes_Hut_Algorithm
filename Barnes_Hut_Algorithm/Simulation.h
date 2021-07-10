#pragma once

#include "SFML/Graphics.hpp"

#include <algorithm>
#include <vector>
#include "MyRandom.h"
#include "Map.h"
#include "Entity.h"
#include "Node.h"
#include "Camera.h"

class Simulation {
private:
	Map map;
	Camera camera;

	const size_t ENTITY_COUNT = 32;
	bool draw_tree;
	std::vector<CircEntity *> entities;

	QuadTree *quad_tree;

	size_t followed_i;
	sf::RenderWindow* window;

	void generateEntities();
	//sf::Vector2f getSmallCoordinates();
	//sf::Vector2f getBigCoordinates();

	void toggleTree();
	void toggleAcc();
	void toggleBarnesHut();

	void followPrevious();
	void followNext();

	void pollEvents();
	void update();
	void render();
public:
	static sf::Font font;

	//Constructors and destructors
	Simulation(size_t entity_count);
	~Simulation();

	void start();
	bool isRunning();
};

