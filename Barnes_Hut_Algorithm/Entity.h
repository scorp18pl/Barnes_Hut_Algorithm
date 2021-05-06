#pragma once
#include "SFML/Graphics.hpp"

class Entity {
protected:
	sf::Vector2f position, velocity, acceleration;
public:
	//Constructors and destructors
	Entity(sf::Vector2f position, sf::Vector2f velocity);
	~Entity();
};

class GravEntity : Entity {
private:
	float mass;
public:
	void update();

	GravEntity(sf::Vector2f position, sf::Vector2f velocity, float mass);
	~GravEntity();
};