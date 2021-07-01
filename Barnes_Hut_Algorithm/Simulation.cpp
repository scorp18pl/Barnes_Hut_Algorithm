#include "Simulation.h"

bool Simulation::isRunning() {
	return this->window->isOpen();
}

void Simulation::generateEntities() {
	for (size_t i = 0; i < ENTITY_COUNT; i++) {
		sf::Vector2f position, velocity;
		position.x = MyRandom::getRandomFloat(0.f, (float)this->window->getSize().x);
		position.y = MyRandom::getRandomFloat(0.f, (float)this->window->getSize().y);

		float v = 1.0f;

		velocity.x = MyRandom::getRandomFloat(-v, v);
		velocity.y = MyRandom::getRandomFloat(-v, v);

		float radius = MyRandom::getRandomFloat(1.f, 5.f);

		entities[i] = CircGravEntity(position, velocity, radius * radius, radius);
	}
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
	
	//for (size_t i = 0; i < ENTITY_COUNT; i++)
	//	this->map->
	for (size_t i = 0; i < ENTITY_COUNT; i++)
		entities[i].update();

	this->quad_tree->update();

	this->quad_tree->calculateForces();
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

	this->entities = new CircGravEntity[ENTITY_COUNT];

	this->quad_tree = new QuadTree(nullptr, this->map.getStartingPosition(), this->map.getSide());

	//Window Setup
	this->window = new sf::RenderWindow(sf::VideoMode(1280, 720), "Simulation", sf::Style::Titlebar | sf::Style::Close);

}

Simulation::~Simulation() {
	delete window;
	delete [] entities;
}

void Simulation::start() {
	generateEntities();
	this->quad_tree->build(this->entities, this->ENTITY_COUNT);

	while (isRunning()) {
		update();
		render();
	}
}
