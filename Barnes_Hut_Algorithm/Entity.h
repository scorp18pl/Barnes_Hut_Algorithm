#pragma once

#include <iostream>
#include <cmath>
#include <cassert>
#include <string>

#include "SFML/Graphics.hpp"
#include "OmniMath.h"
#include "Tracker.h"

class Entity {
protected:
	sf::Font font;
	static bool debug_draw;
	static bool tracker_draw;

	static const float G;
	float mass;
	sf::Vector2f position, velocity, acceleration;
	Tracker tracker;
	
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

	void applyForce(sf::Vector2f force);
	void applyForce(float f_x, float f_y);

	void zeroAcc();
	virtual void update();

	//drawing
	virtual void draw(sf::RenderWindow *window);
	static void toggleDebugDraw();
	void clearTracker();
	static void toggleTrackerDraw();

	//Constructors and destructors
	Entity(sf::Vector2f position, sf::Vector2f velocity, float mass);
};

class CircEntity : public Entity{
private:
	sf::CircleShape shape;
	void updateShapePosition();

public:
	void draw(sf::RenderWindow *window) override;

	CircEntity();

	CircEntity(sf::Vector2f position, sf::Vector2f velocity, float mass,
				   float radius = 4.0f, sf::Color color = sf::Color::White);

	~CircEntity();

	void update() override;
};