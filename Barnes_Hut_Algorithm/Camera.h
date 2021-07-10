#pragma once

#include "Entity.h"

enum class Direction {
	UP, RIGHT, DOWN, LEFT
};

class Camera : public Entity{
private:
	static float FRICTION, FORCE, MAX_V;

	bool follow;
	Entity *followed;

	sf::View view;
public:
	//Getters and setters
	sf::View getView();
	Entity *getFollowed();

	void clearMovement();
	void setFollowed(Entity *followed);

	//Various
	void toggleFollow();

	void zoom(float z);
	void move(Direction d);
	void update() override;

	//Constructors & Destructors
	Camera();
	Camera(sf::RenderWindow *window);

	~Camera();
};

