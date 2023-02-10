#pragma once

#include "SFML/Graphics.hpp"

#include <algorithm>
#include <vector>
#include <memory>
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
	std::vector<std::shared_ptr<CircEntity>> entities;

	QuadTree *quad_tree;

	size_t followed_i;
	sf::RenderWindow* window;

	void generateEntities();

	void toggleTree();
	void toggleAcc();
	void toggleTrackers();
	void toggleTrackerLimit();
	void toggleBarnesHut();

	void followPrevious();
	void followNext();
	void disableFollow();
	void clearTrackers();

	void pollEvents();
	void update();
	void render();

    std::vector<Entity *> getEntityPointers();
public:
	//Constructors and destructors
	Simulation(size_t entity_count);
	~Simulation();

	void start();
	bool isRunning();
};

