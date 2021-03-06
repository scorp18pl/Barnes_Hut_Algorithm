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

void Simulation::toggleTree() {
	this->draw_tree = !this->draw_tree;
}

void Simulation::toggleBarnesHut() {
	this->quad_tree->toggleBarnesHut();
	std::cout << "Toggled BarnesHut" << std::endl;
}

void Simulation::followPrevious() {
	--this->followed_i %= this->entities.size();
	this->camera.setFollowed(this->entities[this->followed_i]);
}

void Simulation::followNext() {
	++this->followed_i %= this->entities.size();
	this->camera.setFollowed(this->entities[this->followed_i]);
}

void Simulation::disableFollow() {
	this->camera.disableFollow();
}

void Simulation::clearTrackers() {
	for (size_t i = 0; i < this->entities.size(); i++)
		this->entities[i]->clearTracker();
}

void Simulation::toggleAcc() {
	Entity::toggleDebugDraw();
}

void Simulation::toggleTrackers() {
	Entity::toggleTrackerDraw();
}

void Simulation::toggleTrackerLimit() {
	Tracker::toggleLimit();
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
						this->camera.move(Direction::LEFT);
						break;
					case sf::Keyboard::W:
						this->camera.move(Direction::UP);
						break;
					case sf::Keyboard::S:
						this->camera.move(Direction::DOWN);
						break;
					case sf::Keyboard::D:
						this->camera.move(Direction::RIGHT);
						break;
					case sf::Keyboard::P:
						this->camera.zoom(1.2f);
						break;
					case sf::Keyboard::L:
						this->camera.zoom(0.8f);
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
					case sf::Keyboard::F:
						toggleAcc();
						break;
					case sf::Keyboard::B:
						toggleBarnesHut();
						break;
					case sf::Keyboard::Q:
						followPrevious();
						break;
					case sf::Keyboard::E:
						followNext();
						break;
					case sf::Keyboard::Z:
						disableFollow();
						break;
					case sf::Keyboard::G:
						toggleTrackers();
						break;
					case sf::Keyboard::C:
						clearTrackers();
						break;
					case sf::Keyboard::I:
						toggleTrackerLimit();
						break;
					default:
						break;
				}
			default:
				break;
		}
	}
}

void Simulation::update() {
	pollEvents();

	for (size_t i = 0; i < entities.size(); i++)
		entities[i]->update();

	this->camera.update();
	this->window->setView(this->camera.getView());

	for (size_t i = 0; i < entities.size(); i++)
		if (!this->map.isInside(this->entities[i]->getPosition())) {
			this->entities[i]->disable();
			this->entities.erase(this->entities.begin() + i);
			
			if (this->camera.getFollowed() == this->entities[i]) {
				this->camera.setFollowed(nullptr);
				this->camera.toggleFollow();
			}
			
			i--;
		}

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

void Simulation::start() {
	generateEntities();
	this->quad_tree->build(this->entities);

	while (isRunning()) {
		update();
		render();
	}
}

Simulation::Simulation(size_t entity_count) 
	:ENTITY_COUNT(entity_count), draw_tree(false), followed_i(0UL) {

	this->entities = std::vector<CircEntity *>();

	this->quad_tree = new QuadTree(nullptr, this->map.getStartingPosition(), this->map.getSide());

	this->window = new sf::RenderWindow(sf::VideoMode(1920, 1080), "Simulation", sf::Style::Titlebar | sf::Style::Close);
	this->window->setFramerateLimit(60);
	this->camera = Camera(this->window);
	this->camera.zoom(1.0e-3f);
}

Simulation::~Simulation() {
	delete quad_tree;
	delete window;
}
