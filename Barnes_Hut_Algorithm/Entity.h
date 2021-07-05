#pragma once
#include "SFML/Graphics.hpp"
#include <iostream>

class Entity {
protected:
	float mass;
	sf::Vector2f position, velocity, acceleration;
public:
	//Constructors and destructors
	Entity(sf::Vector2f position, sf::Vector2f velocity, float mass);

	//Getters and setters
	float getMass();
	sf::Vector2f getPosition();

	//Other public methods
	float getDistance(sf::Vector2f position);
	sf::Vector2f GForce(float mass, sf::Vector2f position);
	void accelerate(sf::Vector2f force);
	void update();
	virtual void draw(sf::RenderWindow *window);
};

class CircEntity : public Entity{
private:
	sf::CircleShape shape;
	void updateShapePosition();

public:
	void draw(sf::RenderWindow *window) override;

	CircEntity();

	CircEntity(sf::Vector2f position, sf::Vector2f velocity, float mass,
				   float radius = 4.f, sf::Color color = sf::Color::White);

	~CircEntity();

	void update();
};