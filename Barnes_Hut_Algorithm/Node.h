#pragma once
#include <iostream>
#include <stdbool.h>
#include <stack>
#include "Entity.h"

enum class Quadrant{
	ERROR = -1, NORTH_WEST, NORTH_EAST, SOUTH_WEST, SOUTH_EAST
};

class Node {
//Static
public:
	static const size_t CHILDREN_COUNT = 4;

//Non - Static
private:
	//Node attributes
	sf::Vector2f position;
	float side_length;
	float mass;
	sf::Vector2f mass_center;

	CircGravEntity* entity;

	//Parent
	Node *parent;

	//Children
	Node *children[CHILDREN_COUNT];
	
	sf::RectangleShape shape;
	
	//Methods
	void setChildToNull(Quadrant quadrant);
	
	bool isEmpty();

	bool hasNoChildren();
	bool otherAreNull(Quadrant quadrant);
	bool hasOnlyOneChild();

	bool outOfReach(sf::Vector2f position);
	bool isInside(CircGravEntity *entity);
	bool isEntityInside();
	bool isSimplest();

	sf::Vector2f getChildPosition(Quadrant q);

	Quadrant checkQuadrant(sf::Vector2f position);
public:
	void update();
	void pushQ(CircGravEntity *entity, Quadrant q);
	void push(CircGravEntity *entity);
	
	void addMass(float mass);

	void moveUp(CircGravEntity *entity, sf::Vector2f child_position, bool set_to_nullptr);
	void draw(sf::RenderWindow *window);

	int countEntities();
	float sumMass();

	//Constructors and destructors
	Node(Node *parent, sf::Vector2f position, float side_length);
	~Node();
};

class QuadTree {
private:
	Node *tree;
	std::stack<Node *> stack;

public:
	//Getters and setters
	Node *getTree();

	void update();
	void calculateForces();
	void build(CircGravEntity entities[], size_t size);
	void draw(sf::RenderWindow *window);

	//Constructors and Destructors
	QuadTree();
	QuadTree(Node *parent, sf::Vector2f position, float side_length);
	~QuadTree();
};