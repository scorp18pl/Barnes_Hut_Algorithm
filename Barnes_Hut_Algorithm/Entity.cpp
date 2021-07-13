#include "Entity.h"

const float Entity::G = 1e-3f;

bool Entity::debug_draw = false;
bool Entity::tracker_draw = false;

Entity::Entity(sf::Vector2f position, sf::Vector2f velocity, float mass)
	:position(position), velocity(velocity), mass(mass),
	acceleration(0.0f, 0.0f) {
}

float Entity::getMass() {
	return this->mass;
}

sf::Vector2f Entity::getPosition() {
	return this->position;
}

void Entity::disable() {
	this->mass = -1.0f;
}

bool Entity::isDisabled() {
	return this->mass == -1.0f;
}

float Entity::getDistance(sf::Vector2f position) {
	float x, y;
	x = this->position.x - position.x;
	y = this->position.y - position.y;

	return (float)sqrt(x * x + y * y);
}

sf::Vector2f Entity::GForce(float mass, sf::Vector2f position) {
	//Calculating the tangent;
	float x_diff, y_diff;
	x_diff = position.x - this->position.x;
	y_diff = position.y - this->position.y;
	
	//Calculating the force vector norm
	float square_dist, force;
	square_dist = x_diff * x_diff + y_diff * y_diff;

	if (square_dist < 1e-4f)
		return sf::Vector2f(0.0f, 0.0f);
	
	force = Entity::G * mass * this->mass / square_dist;

	//Determining the force vector coordinate signs
	float sign_x, sign_y;
	sign_x = (x_diff > 0) * 2.0f + -1.0f;
	sign_y = (y_diff > 0) * 2.0f + -1.0f;

	//Calculating the force vector
	sf::Vector2f force_vec;
	float dist_inv;
	
	assert(square_dist != 0.0f);
	dist_inv = Q_rsqrt(square_dist);

	float base = force * dist_inv;
	force_vec.x = base * std::abs(x_diff) * sign_x;
	force_vec.y = base * std::abs(y_diff) * sign_y;

	return force_vec;
}

std::string Entity::toString() {
	std::string s = "";

	s += "Mass: ";
	s += std::to_string(this->mass);
	s += '\n';

	s += "Position:";
	s += '\n';
	s += std::to_string(this->position.x);
	s += ", ";
	s += std::to_string(this->position.y);
	s += '\n';

	s += "Velocity:";
	s += '\n';
	s += std::to_string(this->velocity.x);
	s += ", ";
	s += std::to_string(this->velocity.y);
	s += '\n';

	s += "Acceleration:";
	s += '\n';
	s += std::to_string(this->acceleration.x);
	s += ", ";
	s += std::to_string(this->acceleration.y);
	s += '\n';

	return s;
}

void Entity::update() {
	this->position.x += this->velocity.x;
	this->position.y += this->velocity.y;

	this->velocity.x += this->acceleration.x;
	this->velocity.y += this->acceleration.y;

	this->tracker.addVertex(this->position);
	this->tracker.addVertex(this->position);
}

void Entity::draw(sf::RenderWindow *window) {
}

void Entity::toggleDebugDraw() {
	Entity::debug_draw = !Entity::debug_draw;
}

void Entity::clearTracker() {
	this->tracker.clear();
}

void Entity::toggleTrackerDraw() {
	Entity::tracker_draw = !Entity::tracker_draw;
}

void CircEntity::draw(sf::RenderWindow* window) {
	window->draw(this->shape);

	if (Entity::tracker_draw)
		this->tracker.draw(window);

	if (!Entity::debug_draw)
		return;
	{
		sf::Vector2f acc = this->position;
		float norm = Q_rsqrt(this->acceleration.x * this->acceleration.x +
							 this->acceleration.y * this->acceleration.y);
		acc.x += this->shape.getRadius() * this->acceleration.x * norm;
		acc.y += this->shape.getRadius() * this->acceleration.y * norm;

		sf::Vertex line[2] = {
			sf::Vertex(this->position, sf::Color::Red),
			sf::Vertex(acc, sf::Color::Red)
		};

		window->draw(line, 2, sf::Lines);
	}

	{
		sf::Vector2f vel = this->position;
		float norm = Q_rsqrt(this->velocity.x * this->velocity.x +
							 this->velocity.y * this->velocity.y);
		vel.x += this->shape.getRadius() * this->velocity.x * norm;
		vel.y += this->shape.getRadius() * this->velocity.y * norm;

		sf::Vertex line[2] = {
			sf::Vertex(this->position, sf::Color::Green),
			sf::Vertex(vel, sf::Color::Green)
		};

		window->draw(line, 2, sf::Lines);
	}
}

void Entity::accelerate(sf::Vector2f force) {
	this->acceleration.x += force.x / this->mass;
	this->acceleration.y += force.y / this->mass;
}

void Entity::accelerate(float f_x, float f_y) {
	accelerate(sf::Vector2f(f_x, f_y));
}

void Entity::zeroAcc() {
	this->acceleration = sf::Vector2f(0.0f, 0.0f);
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
	:Entity(position, velocity, mass) {
	this->shape.setPosition(Entity::position);
	this->shape.setRadius(radius);
	this->shape.setFillColor(color);
	this->font = font;
}

CircEntity::~CircEntity() {
}

void CircEntity::update() {
	Entity::update();
	this->updateShapePosition();
}

