#pragma once
#include <iostream>
#include <stdbool.h>
#include "Entity.h"

enum class Quadrant{
	ERROR = -1, NORTH_WEST, NORTH_EAST, SOUTH_WEST, SOUTH_EAST
};

class Node {
private:
	//Node attributes
	sf::Vector2f position;
	float side_length;
	float mass;

	CircGravEntity* entity;

	//Children
	Node* north_west;
	Node* north_east;
	Node* south_west;
	Node* south_east;
	
	sf::RectangleShape shape;
	
	//Methods
	bool isEmpty();
	bool hasNoChildren();

public:
	//Constructors and destructors
	Node(sf::Vector2f position, float side_length);
	~Node();

	Quadrant checkQuadrant(CircGravEntity* entity);
	void pushQ(CircGravEntity *entity, Quadrant q);
	void push(CircGravEntity *entity);
	void draw(sf::RenderWindow *window);
};

class QuadTree {
private:
	Node *tree;

public:
	//Constructors and Destructors
	QuadTree();
	QuadTree(sf::Vector2f position, float side_length);
	~QuadTree();

	//Getters and setters
	Node *getTree();

	void build(CircGravEntity entities[], size_t size);
	void draw(sf::RenderWindow *window);
};

