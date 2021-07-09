#include "Simulation.h"

bool Simulation::isRunning() {
	return this->window->isOpen();
}

void Simulation::generateEntities() {
	for (size_t i = 0; i < ENTITY_COUNT; i++) {
		sf::Vector2f position, velocity;

		float div = 1e2f;

		position.x = MyRandom::getRandomFloat(this->map.getStartingPosition().x / div, this->map.getEndingPosition().x / div);
		position.y = MyRandom::getRandomFloat(this->map.getStartingPosition().y / div, this->map.getEndingPosition().y / div);

		float v = 1e3f;

		velocity.x = MyRandom::getRandomFloat(-v, v);
		velocity.y = MyRandom::getRandomFloat(-v, v);

		float radius = MyRandom::getRandomFloat(1.0f, 1e3f);

		entities.push_back(new CircEntity(position, velocity, radius * radius * radius, radius));
	}

	entities.push_back(new CircEntity(sf::Vector2f(0.0f, 0.0f), 
									  sf::Vector2f(0.0f, 0.0f), 100000.0f * 100000.0f * 100000.0f, 100000.0f));
}

void Simulation::addCamVel(sf::Vector2f v) {
	this->camera_velocity += v;
}

void Simulation::clearCamVel() {
	this->camera_velocity = sf::Vector2f(0.0f, 0.0f);
}

void Simulation::setViewPosition(sf::Vector2f v) {
	sf::View view = this->window->getView();
	view.setCenter(v);
	this->window->setView(view);
}

void Simulation::moveView(sf::Vector2f v) {
	sf::View view = this->window->getView();
	sf::Vector2f center = view.getCenter();

	center.x += v.x * this->window->getSize().x / 1000.0f;
	center.y += v.y * this->window->getSize().y / 1000.0f;

	view.setCenter(center);
	this->window->setView(view);
}

void Simulation::zoomView(float delta) {
	if (delta == 0.0f)
		return;

	sf::View view = this->window->getView();
	sf::Vector2f size = view.getSize();
	std::cout << "size " << size.x << std::endl;
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

void Simulation::toggleCenter() {
	this->center = !this->center;
}

void Simulation::toggleTree() {
	this->draw_tree = !this->draw_tree;
}

void Simulation::toggleBarnesHut() {
	this->quad_tree->toggleBarnesHut();
	std::cout << "Toggled BarnesHut" << std::endl;
}

void Simulation::toggleAcc() {
	for (size_t i = 0; i < this->entities.size(); i++)
		this->entities[i]->toggleDrawAcc();
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
						addCamVel(sf::Vector2f(-2.0f, 0.0f));
						break;
					case sf::Keyboard::W:
						addCamVel(sf::Vector2f(0.0f, -2.0f));
						break;
					case sf::Keyboard::S:
						addCamVel(sf::Vector2f(0.0f, 2.0f));
						break;
					case sf::Keyboard::D:
						addCamVel(sf::Vector2f(2.0f, 0.0f));
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
					case sf::Keyboard::A:
						clearCamVel();
						break;
					case sf::Keyboard::W:
						clearCamVel();
						break;
					case sf::Keyboard::S:
						clearCamVel();
						break;
					case sf::Keyboard::D:
						clearCamVel();
						break;
					case sf::Keyboard::Q:
				
					case sf::Keyboard::T:
						toggleTree();
						break;
					case sf::Keyboard::F:
						toggleAcc();
						break;
					case sf::Keyboard::B:
						toggleBarnesHut();
						break;
					default:
						break;
				}
		}
		moveView(this->camera_velocity);
	}
}

void Simulation::update() {
	pollEvents();

	for (size_t i = 0; i < entities.size(); i++)
		entities[i]->update();

	for (size_t i = 0; i < entities.size(); i++)
		if (!this->map.isInside(this->entities[i]->getPosition())) {
			this->entities[i]->disable();
			this->entities.erase(this->entities.begin() + i);
			
			if (this->center_entity == this->entities[i]) {
				this->center_entity = nullptr;
				this->center = false;
			}
			
			i--;
		}

	std::cout << "There are " << this->entities.size() << " entities." << std::endl;

	this->quad_tree->update();

	for (size_t i = 0; i < entities.size(); i++)
		this->entities[i]->zeroAcc();

	for (size_t i = 0; i < entities.size(); i++)
		this->quad_tree->calculateForces(this->entities[i]);
}

void Simulation::render() {
	window->clear();

	for (size_t i = 0; i < entities.size(); i++)
		entities[i]->draw(this->window);
	
	if (this->draw_tree)
		quad_tree->draw(this->window);

	window->display();
}

Simulation::Simulation(size_t entity_count) 
	:ENTITY_COUNT(entity_count), draw_tree(false), camera_velocity(sf::Vector2f(0.0f, 0.0f)), 
	center(false), center_entity(nullptr) {

	this->entities = std::vector<CircEntity *>();

	this->quad_tree = new QuadTree(nullptr, this->map.getStartingPosition(), this->map.getSide());

	//Window Setup
	//this->window = new sf::RenderWindow(sf::VideoMode(1280, 720), "Simulation", sf::Style::Titlebar | sf::Style::Close);
	this->window = new sf::RenderWindow(sf::VideoMode(1920, 1080), "Simulation", sf::Style::Titlebar | sf::Style::Close);
	this->window->setFramerateLimit(60);

}

Simulation::~Simulation() {
	delete quad_tree;
	delete window;
}

void Simulation::start() {
	generateEntities();
	this->quad_tree->build(this->entities);

	while (isRunning()) {
		update();
		render();
	}
}
