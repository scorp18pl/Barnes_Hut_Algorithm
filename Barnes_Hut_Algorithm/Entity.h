#pragma once
#include "SFML/Graphics.hpp"
#include <iostream>

class Entity {
protected:
	sf::Vector2f position, velocity, acceleration;
public:
	//Constructors and destructors
	Entity(sf::Vector2f position, sf::Vector2f velocity);

	//Getters and setters
	sf::Vector2f getPosition();

	//Other public methods
	void update();
};

class GravEntity : public Entity {
protected:
	float mass;
public:
	//Constructors and destructors
	GravEntity(sf::Vector2f position, sf::Vector2f velocity, float mass);
	
	//Getters and setters
	float getMass();

	//Other public methods
	void update();
};

class CircEntity {
protected:
	sf::CircleShape shape;
public:
	CircEntity(float radius = 4.f, sf::Color color = sf::Color::White);
	void draw(sf::RenderWindow *window);
};

class CircGravEntity : public GravEntity, public CircEntity{
private:
	void updateShapePosition();

public:
	CircGravEntity();

	CircGravEntity(sf::Vector2f position, sf::Vector2f velocity, float mass,
				   float radius = 4.f, sf::Color color = sf::Color::White);

	~CircGravEntity();

	void update();
};