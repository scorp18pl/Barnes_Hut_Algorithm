#pragma once
#include <iostream>
#include <stdbool.h>
#include <stack>
#include <cassert>

#include "Entity.h"

enum Quadrant : int {
    OUT_OF_BOUNDS = -1,
	NORTH_WEST = 0,
    NORTH_EAST = 1,
    SOUTH_WEST = 2,
    SOUTH_EAST = 3,
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

	Entity* assignedEntity;

	//Parent
	Node *parent;

	//Children
	Node *subquadrants[CHILDREN_COUNT];
	
	sf::RectangleShape shape;
	
	//Methods
    //! Sets a quadrant child node to nullptr.
	void setChildToNull(Quadrant quadrant);

    //! The node is empty when it has no assigned entity.
	bool isEmpty();

    bool isRoot() const;

    //! Returns whether or not a node has subquadrants.
	bool hasNoChildren();

    //! Returns whether or not quadrant is the only
    //! child of the node.
	bool otherAreNull(Quadrant quadrant);

    //! Returns whether or not the node has only a one child.
	bool hasOnlyOneChild();

	bool outOfReach(sf::Vector2f position);

    //! Returns whether or not given entity is contained
    //! within the bounds of the node.
	bool isInside(Entity *entity);

    //! Returns whether or not entity assigned to the node
    //! is contained within the bounds of the node.
	bool isEntityInside();

    //! Returns whether or not a leaf node should be deleted
    //! and the entity moved up the tree.
	bool isSimplest();

	sf::Vector2f getChildPosition(Quadrant q);

    //! Returns the quadrant in which given position is
    //! situated. Considered quadrants' coordinate intervals
    //! are in the form [a, b) and [b, c] (for each).
	Quadrant selectQuadrant(sf::Vector2f position);
public:
	void update();
	void updateMass();
	void updateCenterOfMass();

    void ensureQuadrantExists(Quadrant quadrant);
	void push(Entity *entity);

    //! Recursively moves the entity up the quadtree until it
    //! it is contained within the node .
    //! @param removeSubquadrant Should the subquadrant be removed?
	void moveUp(Entity *entity, sf::Vector2f subquadrantPosition, bool removeSubquadrant);
	void branchRemove(sf::Vector2f child_position, bool set_to_nullptr);

	void setOutlineThickness(float thickness);
	void draw(sf::RenderWindow *window);

    //! Recursively calculates forces for all
    //! bodies.
	void calculateForce(Entity *entity, bool barnes_hut);

	bool isCloseEnough(Entity *entity);

	int countEntities();
	float getMass();
	sf::Vector2f getCenterOfMass();

	//Constructors and destructors
	Node(Node *parent, sf::Vector2f position, float side_length);
	~Node();
};

class QuadTree {
private:
	bool barnes_hut;
	static std::stack<Node *> stack;
	Node *tree;

	static void stackClear();
public:

	//Getters and setters
	Node *getTree();

	static void stackPush(Node *node);
	void update();

	void toggleBarnesHut();
	void calculateForces(Entity *entity);
	
	void build(std::vector<Entity *> entities);
	void draw(sf::RenderWindow *window);

	//Constructors and Destructors
	QuadTree();
	QuadTree(sf::Vector2f position, float side_length);
	~QuadTree();
};

