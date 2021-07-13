#include "Tracker.h"

bool Tracker::limit = true;

void Tracker::addVertex(sf::Vector2f position) {
	if (vertices.size() > Tracker::MAX_LENGTH) {
		this->vertices.erase(this->vertices.begin());
	}

	sf::Vertex v;
	v.position = position;
	v.color = sf::Color(32, 94, 37, 255);

	this->vertices.push_back(v);
}

void Tracker::clear() {
	this->vertices.clear();
}

void Tracker::draw(sf::RenderWindow *window) {
	if (this->vertices.size() == 0UL)
		return;

	window->draw(&this->vertices[0], this->vertices.size(), sf::Lines);
}

void Tracker::toggleLimit() {
	Tracker::limit = !Tracker::limit;
}

Tracker::Tracker() {
}

Tracker::~Tracker() {
}
