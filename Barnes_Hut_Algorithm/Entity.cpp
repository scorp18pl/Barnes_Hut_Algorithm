#include "Entity.h"

Entity::Entity(sf::Vector2f position, sf::Vector2f velocity, float mass)
	:position(position), velocity(velocity), mass(mass) {
	this->acceleration.x = 0.f;
	this->acceleration.y = 0.f;
}

float Entity::getMass() {
	return this->mass;
}

sf::Vector2f Entity::getPosition() {
	return this->position;
}

float Entity::getDistance(sf::Vector2f position) {
	float x, y;
	x = this->position.x - position.x;
	y = this->position.y - position.y;

	return (float)sqrt(x * x + y * y);
}

sf::Vector2f Entity::GForce(float mass, sf::Vector2f position) {
	return sf::Vector2f(0.0f, 0.0f);
}

void Entity::update() {
	this->position.x += this->velocity.x;
	this->position.y += this->velocity.y;

	this->velocity.x += this->acceleration.x;
	this->velocity.y += this->acceleration.y;

	this->acceleration = sf::Vector2f(0.f, 0.f);
}

void Entity::draw(sf::RenderWindow *window) {
}

void CircEntity::draw(sf::RenderWindow* window) {
	window->draw(this->shape);
}

void Entity::accelerate(sf::Vector2f force) {
	this->acceleration.x += force.x / this->mass;
	this->acceleration.y += force.y / this->mass;
}

void CircEntity::updateShapePosition() {
	sf::Vector2f position = this->position;
	float r = this->shape.getRadius();
	position.x -= r;
	position.y -= r;

	this->shape.setPosition(position);
}

CircEntity::CircEntity()
	:CircEntity(sf::Vector2f(0.f, 0.f), sf::Vector2f(0.f, 0.f), 0.f) {
}

CircEntity::CircEntity(sf::Vector2f position, sf::Vector2f velocity, float mass,
							   float radius, sf::Color color)
	: shape(sf::CircleShape(radius)), Entity(position, velocity, mass) {
	this->shape.setPosition(Entity::position);
	this->shape.setRadius(radius);
	this->shape.setFillColor(color);
}

CircEntity::~CircEntity() {
}

void CircEntity::update() {
	Entity::update();
	this->updateShapePosition();
}

