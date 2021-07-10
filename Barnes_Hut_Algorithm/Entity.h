#pragma once

#include <iostream>
#include <cmath>
#include <cassert>
#include <string>

#include "SFML/Graphics.hpp"
#include "OmniMath.h"

class Entity {
protected:
	sf::Font font;
	bool debug_draw;

	static const float G;
	float mass;
	sf::Vector2f position, velocity, acceleration;
public:
	//Getters and setters
	float getMass();
	sf::Vector2f getPosition();
	void disable();

	//Other public methods
	bool isDisabled();
	float getDistance(sf::Vector2f position);
	sf::Vector2f GForce(float mass, sf::Vector2f position);
	std::string toString();

	void accelerate(sf::Vector2f force);
	void accelerate(float f_x, float f_y);

	void zeroAcc();
	virtual void update();

	//drawing
	virtual void draw(sf::RenderWindow *window);
	virtual void toggleDrawAcc();

	//Constructors and destructors
	Entity(sf::Vector2f position, sf::Vector2f velocity, float mass, sf::Font font);
};

class CircEntity : public Entity{
private:
	sf::CircleShape shape;
	void updateShapePosition();

public:
	void draw(sf::RenderWindow *window) override;

	CircEntity();

	CircEntity(sf::Vector2f position, sf::Vector2f velocity, float mass, sf::Font font,
				   float radius = 4.0f, sf::Color color = sf::Color::White);

	~CircEntity();

	void update();
};