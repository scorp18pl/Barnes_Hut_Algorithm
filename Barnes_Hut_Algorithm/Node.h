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
	static const float PHI;

//Non - Static
private:
	//Node attributes
	sf::Vector2f position;
	float side_length;
	float mass;
	sf::Vector2f center_of_mass;

	Entity* entity;

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
	bool isInside(Entity *entity);
	bool isEntityInside();
	bool isSimplest();

	sf::Vector2f getChildPosition(Quadrant q);
	Quadrant checkQuadrant(sf::Vector2f position);
public:
	void update();
	void updateMass();
	void updateCenterOfMass();

	void pushQ(Entity *entity, Quadrant q);
	void push(Entity *entity);

	void moveUp(Entity *entity, sf::Vector2f child_position, bool set_to_nullptr);
	void draw(sf::RenderWindow *window);

	void calculateForce(Entity entity);

	bool isFarEnough(Entity entity);

	int countEntities();
	float getMass();
	sf::Vector2f getCenterOfMass();

	//Constructors and destructors
	Node(Node *parent, sf::Vector2f position, float side_length);
	~Node();
};

class QuadTree {
private:
	static std::stack<Node *> stack;
	Node *tree;

	static void stackClear();
public:

	//Getters and setters
	Node *getTree();

	static void stackPush(Node *node);
	void update();
	void calculateForces(Entity entity);
	void build(Entity entities[], size_t size);
	void draw(sf::RenderWindow *window);

	//Constructors and Destructors
	QuadTree();
	QuadTree(Node *parent, sf::Vector2f position, float side_length);
	~QuadTree();
};

