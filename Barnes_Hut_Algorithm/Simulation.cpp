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

		entities.emplace_back(new CircEntity(position, velocity, radius * radius * radius, radius));
	}

	entities.emplace_back(new CircEntity(sf::Vector2f(0.0f, 0.0f),
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
	--followed_i %= entities.size();
	camera.setFollowed(entities[followed_i].get());
}

void Simulation::followNext() {
	++followed_i %= entities.size();
	camera.setFollowed(entities[followed_i].get());
}

void Simulation::disableFollow() {
	camera.disableFollow();
}

void Simulation::clearTrackers() {
	for (size_t i = 0; i < entities.size(); i++)
		entities[i]->clearTracker();
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

	camera.update();
	window->setView(camera.getView());

	for (auto entity = entities.begin(); entity != entities.end(); ++entity)
		if ((*entity)->isDisabled()) {
			if (camera.getFollowed() == (*entity).get()) {
                camera.setFollowed(nullptr);
				camera.toggleFollow();
			}

            entities.erase(entity);
		}

	this->quad_tree->update();

	for (size_t i = 0; i < entities.size(); i++)
		entities[i]->zeroAcc();

	for (size_t i = 0; i < entities.size(); i++)
		quad_tree->calculateForces(entities[i].get());
}

void Simulation::render() {
	window->clear();

	for (size_t i = 0; i < entities.size(); i++)
		entities[i]->draw(window);
	
	if (draw_tree)
		quad_tree->draw(window);

	window->display();
}

std::vector<Entity *> Simulation::getEntityPointers() {
    std::vector<Entity *> entityPointers;
    entityPointers.reserve(entities.size());

    for (const auto& entity : entities) {
        entityPointers.push_back(entity.get());
    }

    return entityPointers;
}

void Simulation::start() {
	generateEntities();
	quad_tree->build(getEntityPointers());

	while (isRunning()) {
		update();
		render();
	}
}

Simulation::Simulation(size_t entity_count) 
	:ENTITY_COUNT(entity_count), draw_tree(false), followed_i(0UL) {

	quad_tree = new QuadTree(this->map.getStartingPosition(), this->map.getSide());

	window = new sf::RenderWindow(sf::VideoMode(3840, 2160), "Simulation", sf::Style::Titlebar | sf::Style::Close);
	window->setFramerateLimit(60);
	camera = Camera(this->window);
	camera.zoom(1.0e-3f);
}

Simulation::~Simulation() {
	delete quad_tree;
	delete window;
}
