#include "Simulation.h"

bool Simulation::isRunning() {
	return this->window->isOpen();
}

void Simulation::generateEntities() {
	for (size_t i = 0; i < ENTITY_COUNT; i++) {
		sf::Vector2f position, velocity;
		position.x = MyRandom::getRandomFloat(0.f, (float)this->window->getSize().x);
		position.y = MyRandom::getRandomFloat(0.f, (float)this->window->getSize().y);

		float v = 0.001f;

		velocity.x = MyRandom::getRandomFloat(-v, v);
		velocity.y = MyRandom::getRandomFloat(-v, v);

		float radius = MyRandom::getRandomFloat(1.0f, 5.0f);

		entities.push_back(new CircEntity(position, velocity, radius * radius, radius));
	}
}

void Simulation::moveView(sf::Vector2f v) {
	sf::View view = this->window->getView();
	sf::Vector2f center = view.getCenter();

	center.x += v.x;
	center.y += v.y;

	view.setCenter(center);
	this->window->setView(view);
}

void Simulation::zoomView(float delta) {
	if (delta == 0.0f)
		return;

	sf::View view = this->window->getView();
	sf::Vector2f size = view.getSize();
	if (delta > 0) {
		size.x /= delta;
		size.y /= delta;
	}
	else {
		size.x *= -delta;
		size.y *= -delta;
	}

	view.setSize(size);
	this->window->setView(view);
}

void Simulation::toggleTree() {
	if (this->draw_tree == true)
		this->draw_tree = false;
	else
		this->draw_tree = true;
}

void Simulation::pollEvents() {
	sf::Event event;
	while (window->pollEvent(event)) {
		switch (event.type) {
			case sf::Event::Closed:
				window->close();
				break;
			case sf::Event::KeyPressed:
				switch (event.key.code) {
					case sf::Keyboard::A:
						moveView(sf::Vector2f(-4.0f, 0.0f));
						break;
					case sf::Keyboard::W:
						moveView(sf::Vector2f(0.0f, -4.0f));
						break;
					case sf::Keyboard::S:
						moveView(sf::Vector2f(0.0f, 4.0f));
						break;
					case sf::Keyboard::D:
						moveView(sf::Vector2f(4.0f, 0.0f));
						break;
					case sf::Keyboard::P:
						zoomView(2.0f);
						break;
					case sf::Keyboard::L:
						zoomView(-2.0f);
						break;
					default:
						break;
				}
				break;
			case sf::Event::KeyReleased:
				switch (event.key.code) {
					case sf::Keyboard::T:
						toggleTree();
						break;
					default:
						break;
				}
		}
	}
}

void Simulation::update() {
	pollEvents();

	for (size_t i = 0; i < ENTITY_COUNT; i++)
		entities[i]->update();

	for (size_t i = 0; i < ENTITY_COUNT; i++)
		if (!this->map.isInside(this->entities[i]->getPosition())) {
			this->entities[i]->disable();
			this->entities.erase(this->entities.begin() + i);
			i--;
		}

	//std::cout << "There are " << this->entities.size() << " entities." << std::endl;

	for (size_t i = 0; i < ENTITY_COUNT; i++)
		this->entities[i]->zeroAcc();

	this->quad_tree->update();

	for (size_t i = 0; i < ENTITY_COUNT; i++)
		this->quad_tree->calculateForces(this->entities[i]);
}

void Simulation::render() {
	window->clear();

	for (size_t i = 0; i < ENTITY_COUNT; i++)
		entities[i]->draw(this->window);
	
	if (this->draw_tree)
		quad_tree->draw(this->window);

	window->display();
}

Simulation::Simulation(size_t entity_count) 
	:ENTITY_COUNT(entity_count), draw_tree(false) {

	this->entities = std::vector<CircEntity *>();

	this->quad_tree = new QuadTree(nullptr, this->map.getStartingPosition(), this->map.getSide());

	//Window Setup
	this->window = new sf::RenderWindow(sf::VideoMode(1280, 720), "Simulation", sf::Style::Titlebar | sf::Style::Close);

}

Simulation::~Simulation() {
	delete quad_tree;
	delete window;
}

void Simulation::start() {
	generateEntities();
	this->quad_tree->build(this->entities, this->ENTITY_COUNT);

	while (isRunning()) {
		update();
		render();
	}
}
