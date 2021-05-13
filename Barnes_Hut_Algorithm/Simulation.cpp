#include "Simulation.h"

bool Simulation::isRunning() {
	return this->window->isOpen();
}

void Simulation::generateEntities() {
	for (size_t i = 0; i < ENTITY_COUNT; i++) {
		sf::Vector2f position, velocity;
		position.x = MyRandom::getRandomFloat(0.f, (float)window->getSize().x);
		position.y = MyRandom::getRandomFloat(0.f, (float)window->getSize().y);

		float v = 1.f;

		velocity.x = MyRandom::getRandomFloat(-v, v);
		velocity.y = MyRandom::getRandomFloat(-v, v);

		float radius = MyRandom::getRandomFloat(1.f, 5.f);

		entities[i] = CircGravEntity(position, velocity, radius * radius, radius);
	}
}

sf::Vector2f Simulation::getSmallCoordinates() {
	sf::Vector2f return_p = entities[0].getPosition();

	for (size_t i = 0; i < ENTITY_COUNT; i++) {
		sf::Vector2f curr_pos = entities[i].getPosition();
		
		if (curr_pos.x < return_p.x)
			return_p.x = curr_pos.x;

		if (curr_pos.y < return_p.y)
			return_p.y = curr_pos.y;
	}

	return return_p;
}

sf::Vector2f Simulation::getBigCoordinates() {
	sf::Vector2f return_p = entities[0].getPosition();

	for (size_t i = 0; i < ENTITY_COUNT; i++) {
		sf::Vector2f curr_pos = entities[i].getPosition();

		if (curr_pos.x > return_p.x)
			return_p.x = curr_pos.x;

		if (curr_pos.y > return_p.y)
			return_p.y = curr_pos.y;
	}

	return return_p;
}

void Simulation::pollEvents() {
	sf::Event event;
	while (window->pollEvent(event)) {
		switch (event.type) {
			case sf::Event::Closed:
				window->close();
				break;
		}
	}
}

void Simulation::update() {
	pollEvents();

	delete this->quad_tree;

	sf::Vector2f small_pos, bigg_pos;
	small_pos = getSmallCoordinates();
	bigg_pos = getBigCoordinates();

	float side_length = std::max(bigg_pos.x - small_pos.x,
								 bigg_pos.y - small_pos.y);

	this->quad_tree = new QuadTree(small_pos, side_length);
	this->quad_tree->build(entities, ENTITY_COUNT);

	for (size_t i = 0; i < ENTITY_COUNT; i++)
		entities[i].update();
}

void Simulation::render() {
	window->clear();

	for (size_t i = 0; i < ENTITY_COUNT; i++)
		entities[i].draw(this->window);

	quad_tree->draw(this->window);

	window->display();
}

Simulation::Simulation(size_t entity_count) 
	:ENTITY_COUNT(entity_count) {

	//Gravitational Entities Setup
	entities = new CircGravEntity[ENTITY_COUNT];

	//Window Setup

	window = new sf::RenderWindow(sf::VideoMode(1280, 720), "Simulation", sf::Style::Titlebar | sf::Style::Close);
}

Simulation::~Simulation() {
	delete window;
	delete [] entities;
}

void Simulation::start() {
	generateEntities();

	while (isRunning()) {
		update();
		render();
	}
}
