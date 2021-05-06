#include "Simulation.h"

bool Simulation::isRunning() {
	return this->window->isOpen();
}

void Simulation::update() {
	quad_tree.build(this->entities);

	for (size_t i = 0; i < ENTITY_COUNT; i++)
		entities[i].update();
}

void Simulation::render() {
	for (size_t i = 0; i < ENTITY_COUNT; i++)
		entities[i].draw(this->window);
}

Simulation::Simulation() {
	//Gravitational Entities Setup
	this->entities = (GravEntity*)malloc(sizeof(GravEntity) * this->ENTITY_COUNT);

	//Window Setup
	this->window = new sf::RenderWindow;

	const sf::Vector2u size = sf::Vector2u(600u, 600u);
	this->window->setSize(size);
}

Simulation::~Simulation() {
}

void Simulation::start() {
	while (isRunning()) {
		update();
		render();
	}
}
