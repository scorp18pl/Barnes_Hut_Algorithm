#include "Entity.h"

Entity::Entity(sf::Vector2f position, sf::Vector2f velocity)
	:position(position), velocity(velocity) {
	this->acceleration.x = 0;
	this->acceleration.y = 0;
}

Entity::~Entity() {
}

void GravEntity::update() {
	this->position.x += this->velocity.x;
	this->position.y += this->velocity.y;

	this->velocity.x += this->acceleration.x;
	this->velocity.y += this->acceleration.y;
}

GravEntity::GravEntity(sf::Vector2f position, sf::Vector2f velocity, float mass)
	:Entity(position, velocity), mass(mass)	{
}

GravEntity::~GravEntity() {
}
