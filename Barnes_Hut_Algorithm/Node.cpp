#include "Node.h"
#include <cassert>

//Sets a quadrant child node to nullptr.
void Node::setChildToNull(Quadrant quadrant) {
	assert(quadrant != Quadrant::ERROR);

	this->children[(int)quadrant] = nullptr;
}

//Returns whether or not a node is empty 
//(has no entity assigned to it).
inline bool Node::isEmpty() {
	return entity == nullptr;
}

//Returns whether or not a node has children.
inline bool Node::hasNoChildren() {
	for (size_t i = 0; i < Node::CHILDREN_COUNT; i++)
		if (this->children[i] != nullptr)
			return false;
	
	return true;
}

//Returns whether or not quadtrant is the only
//child of the node.
bool Node::otherAreNull(Quadrant quadrant) {
	for (size_t i = 0; i < Node::CHILDREN_COUNT; i++)
		if (this->children[i] != nullptr && i != (size_t)quadrant)
			return false;

	return true;
}

//Returns whether or not the node has only a one child.
bool Node::hasOnlyOneChild() {
	int children_count = 0;
	for (size_t i = 0; i < Node::CHILDREN_COUNT; i++)
		if (this->children[i] != nullptr)
			children_count++;

	return children_count == 1;
}

bool Node::outOfReach(sf::Vector2f position) {
	bool out_of_reach = position.x < this->position.x;
	out_of_reach = out_of_reach || position.y < this->position.y;

	out_of_reach = out_of_reach || position.x >= this->position.x + this->side_length;
	out_of_reach = out_of_reach || position.y >= this->position.y + this->side_length;

	return out_of_reach;
}

//Returns whether or not given entity is contained
//within the bounds of the node.
bool Node::isInside(CircGravEntity *entity) {
	return !outOfReach(entity->getPosition());
}

//Returns whether or not entity assigned to the node
//is contained within the bounds of the node.
bool Node::isEntityInside() {
	assert(!isEmpty());
	return isInside(this->entity);
}

//Returns whether or not a leaf node should be deleted
//and the entity moved up the tree.
bool Node::isSimplest() {
	if (this->parent == nullptr)
		return true;

	return !this->parent->hasOnlyOneChild();
}

sf::Vector2f Node::getChildPosition(Quadrant q) {
	sf::Vector2f return_v = sf::Vector2f(this->position);

	switch (q) {
		case Quadrant::ERROR:
			std::cout << "Invalid q" << std::endl;
			break;
		case Quadrant::NORTH_WEST:
			return return_v;
		case Quadrant::NORTH_EAST:
			return_v.x += this->side_length / 2.0f;
			return return_v;
		case Quadrant::SOUTH_WEST:
			return_v.y += this->side_length / 2.0f;
			return return_v;
		case Quadrant::SOUTH_EAST:
			return_v.x += this->side_length / 2.0f;
			return_v.y += this->side_length / 2.0f;
			return return_v;
	}
}

//Returns the quadrant in which given position is
//situated. Considered quadrants' coordinate intervals
//are in the form [a, b) and [b, c] (for each).
Quadrant Node::checkQuadrant(sf::Vector2f position) {
	if (side_length == 0.0f)
		exit(1);

	if (outOfReach(position)) {
		outOfReach(position);
		return Quadrant::ERROR;
	}

	bool x_less, y_less;

	x_less = position.x < this->position.x + side_length / 2.0f;
	y_less = position.y < this->position.y + side_length / 2.0f;

	if (x_less)
		if (y_less)
			return Quadrant::NORTH_WEST;
		else
			return Quadrant::SOUTH_WEST;
	else
		if (y_less)
			return Quadrant::NORTH_EAST;
	
	return Quadrant::SOUTH_EAST;
}

void Node::update() {
	if (this == nullptr)
		return;

	if (hasNoChildren() && isEmpty()) {
		delete this;
		return;
	}

	for (size_t i = 0; i < Node::CHILDREN_COUNT; i++)
		this->children[i]->update();

	if (isEmpty() || (isEntityInside() && isSimplest()))
		return;

	assert(hasNoChildren());

	CircGravEntity *temp_entity = this->entity;
	moveUp(temp_entity, this->position, hasNoChildren());
	delete this;
}

void Node::pushQ(CircGravEntity *entity, Quadrant q) {
	assert(q != Quadrant::ERROR);

	if (this->children[(int)q] == nullptr) {
		sf::Vector2f position = this->getChildPosition(q);
		float side_length = this->side_length / 2.0f;

		this->children[(int)q] = new Node(this, position, side_length);
	}

	this->children[(int)q]->push(entity);
}

void Node::push(CircGravEntity *entity) {
	assert(entity != nullptr && isInside(entity));

	if (this->isEmpty() && this->hasNoChildren()) {
		this->entity = entity;
		return;
	}

	if (!this->isEmpty()) {
		CircGravEntity *temp_entity = this->entity;

		if (isEntityInside()) {
			this->entity = nullptr;
			this->pushQ(temp_entity, this->checkQuadrant(temp_entity->getPosition()));
		}
		else {
			this->entity = entity;
			parent->moveUp(temp_entity, this->position, false);
			return;
		}
	}


	this->pushQ(entity, this->checkQuadrant(entity->getPosition()));
}

void Node::addMass(float mass) {
	this->mass += mass;
}

//Recursively moves the entity up the quadtree until it
//it is contained within the node .
void Node::moveUp(CircGravEntity *entity, sf::Vector2f child_position, bool set_to_nullptr) {
	if (set_to_nullptr)
		setChildToNull(checkQuadrant(child_position));

	if (isInside(entity) && isSimplest()) {
		this->push(entity);
		return;
	}

	parent->moveUp(entity, this->position, hasNoChildren());
}

void Node::draw(sf::RenderWindow *window) {
	if (this == nullptr)
		return;

	window->draw(this->shape);

	for (size_t i = 0; i < Node::CHILDREN_COUNT; i++)
		this->children[i]->draw(window);
}

int Node::countEntities() {
	if (this == nullptr)
		return 0;

	int sum = 0;
	for (size_t i = 0; i < Node::CHILDREN_COUNT; i++)
		sum += this->children[i]->countEntities();

	if (!this->isEmpty())
		sum++;

	return sum;
}

float Node::sumMass() {
	if (this == nullptr)
		return 0.0f;

	if (!isEmpty())
		return this->mass;

	float sum = 0.0f;
	for (size_t i = 0; i < Node::CHILDREN_COUNT; i++)
		sum += children[i]->sumMass();

	return sum;
}

Node::Node(Node *parent, sf::Vector2f position, float side_length)
	:parent(parent), position(position), side_length(side_length), mass(0.f), entity(nullptr) {
	
	this->shape.setPosition(position);
	this->shape.setSize(sf::Vector2f(side_length, side_length));
	this->shape.setFillColor(sf::Color::Transparent);
	this->shape.setOutlineThickness(1);
	this->shape.setOutlineColor(sf::Color(32, 94, 37, 255));

	for (size_t i = 0; i < Node::CHILDREN_COUNT; i++)
		this->children[i] = nullptr;
}

Node::~Node() {
	for (size_t i = 0; i < Node::CHILDREN_COUNT; i++)
		delete this->children[i];
}

QuadTree::QuadTree() {
	this->tree = nullptr;
}

QuadTree::QuadTree(Node *parent, sf::Vector2f position, float side_length) {
	this->tree = new Node(parent, position, side_length);
}

QuadTree::~QuadTree() {
	delete tree;
}

Node *QuadTree::getTree() {
	return tree;
}

void QuadTree::update() {
	std::cout << "There are " << this->tree->countEntities() << " entities in the tree" << std::endl;
	std::cout << "Tree mass == " << this->tree->sumMass() << std::endl;
	this->tree->update();
}

//Recursively calculates forces for all
//bodies.
void QuadTree::calculateForces() {

}

void QuadTree::build(CircGravEntity entities[], size_t size) {
	for (size_t i = 0; i < size; i++)
		this->tree->push(entities + i);
}

void QuadTree::draw(sf::RenderWindow *window) {
	if (tree != nullptr)
		tree->draw(window);
}
