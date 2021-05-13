#include "Entity.h"

Entity::Entity(sf::Vector2f position, sf::Vector2f velocity)
	:position(position), velocity(velocity) {
	this->acceleration.x = 0.f;
	this->acceleration.y = 0.f;
}

sf::Vector2f Entity::getPosition() {
	return position;
}

void Entity::update() {
	this->position.x += this->velocity.x;
	this->position.y += this->velocity.y;

	this->velocity.x += this->acceleration.x;
	this->velocity.y += this->acceleration.y;

	this->acceleration = sf::Vector2f(0.f, 0.f);
}

GravEntity::GravEntity(sf::Vector2f position, sf::Vector2f velocity, float mass)
	:Entity(position, velocity), mass(mass)	{
}

float GravEntity::getMass() {
	return this->mass;
}

CircEntity::CircEntity(float radius, sf::Color color)
	:shape(sf::CircleShape(radius)) {
	this->shape.setFillColor(color);
}

void CircEntity::draw(sf::RenderWindow* window) {
	window->draw(this->shape);
}

void GravEntity::update() {
	Entity::update();
}

void CircGravEntity::updateShapePosition() {
	sf::Vector2f position = this->position;
	float r = this->shape.getRadius();
	position.x -= r;
	position.y -= r;

	this->shape.setPosition(position);
}

CircGravEntity::CircGravEntity()
	:CircGravEntity(sf::Vector2f(0.f, 0.f), sf::Vector2f(0.f, 0.f), 0.f)
{
}

CircGravEntity::CircGravEntity(sf::Vector2f position, sf::Vector2f velocity, float mass,
							   float radius, sf::Color color)
	:GravEntity(position, velocity, mass), CircEntity(){
	this->shape.setPosition(Entity::position);
}

void CircGravEntity::update() {
	GravEntity::update();
	this->updateShapePosition();
}

