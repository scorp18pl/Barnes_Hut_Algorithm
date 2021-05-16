#include "Node.h"
#include <cassert>

void Node::setChildToNull(Quadrant quadrant) {
	switch (quadrant) {
		case Quadrant::NORTH_WEST:
			this->north_west = nullptr;
			break;
		case Quadrant::NORTH_EAST:
			this->north_east = nullptr;
			break;
		case Quadrant::SOUTH_WEST:
			this->south_west = nullptr;
			break;
		case Quadrant::SOUTH_EAST:
			this->south_east = nullptr;
			break;
		default:
			std::cout << "Invalid quadrant in setChildNull" << std::endl;
			exit(1);
	}
}

inline bool Node::isEmpty() {
	return entity == nullptr;
}

inline bool Node::hasNoChildren() {
	return north_west == north_east && south_west == south_east;
}

bool Node::outOfReach(sf::Vector2f position) {
	bool out_of_reach;

	out_of_reach = position.x < this->position.x;
	out_of_reach = out_of_reach || position.y < this->position.y;

	out_of_reach = out_of_reach || position.x > this->position.x + side_length;
	out_of_reach = out_of_reach || position.y > this->position.y + side_length;

	return out_of_reach;
}

//Returns whether or not given entity is contained
//within the bounds of the node.
bool Node::isInside(CircGravEntity *entity) {
	return !outOfReach(entity->getPosition());
}

bool Node::isEntityInside() {
	assert(!isEmpty());
	return isInside(this->entity);
}

//Returns the quadrant in which given position is
//situated. Considered quadrants' coordinate intervals
//are in the form [a, b) and [b, c] (for each).
Quadrant Node::checkQuadrant(sf::Vector2f position) {
	if (side_length == 0.0f)
		exit(1);

	if (outOfReach(position))
		return Quadrant::ERROR;

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

	this->north_west->update();
	this->north_east->update();
	this->south_west->update();
	this->south_east->update();

	if (isEmpty() || isEntityInside())
		return;

	CircGravEntity *temp_entity = this->entity;
	this->parent->moveUp(temp_entity, this->position, true);
	delete this;
}

void Node::pushQ(CircGravEntity *entity, Quadrant q) {
	sf::Vector2f position = this->position;
	float side_length = this->side_length / 2.0f;

	Node **n;

	switch (q) {
		case Quadrant::NORTH_WEST:
			n = &this->north_west;
			break;

		case Quadrant::NORTH_EAST:
			position.x += side_length;
			n = &this->north_east;
			break;

		case Quadrant::SOUTH_WEST:
			position.y += side_length;
			n = &this->south_west;
			break;

		case Quadrant::SOUTH_EAST:
			position.x += side_length;
			position.y += side_length;
			n = &this->south_east;
			break;

		default:
			return;
	}

	if (*n == nullptr)
		*n = new Node(this, position, side_length);

	(*n)->push(entity);
}

void Node::push(CircGravEntity *entity) {
	assert(entity != nullptr);
	
	mass += entity->getMass();

	if (this->isEmpty() && this->hasNoChildren()) {
		this->entity = entity;
		return;
	}

	if (this->hasNoChildren()) {
		this->pushQ(this->entity, this->checkQuadrant(this->entity->getPosition()));
		this->entity = nullptr;
	}

	this->pushQ(entity, this->checkQuadrant(entity->getPosition()));
}

//Recursively moves the entity up the quadtree until it
//it is contained within the node.
void Node::moveUp(CircGravEntity *entity, sf::Vector2f child_position, bool set_to_nullptr) {
	if (set_to_nullptr)
		setChildToNull(checkQuadrant(child_position));

	if (isInside(entity)) {
		this->push(entity);
		return;
	}

	parent->moveUp(entity, this->position, hasNoChildren());
	
	if (hasNoChildren())
		delete this;
}

void Node::draw(sf::RenderWindow *window) {
	if (this == nullptr)
		return;

	window->draw(shape);

	north_west->draw(window);
	north_east->draw(window);
	south_west->draw(window);
	south_east->draw(window);
}

Node::Node(Node *parent, sf::Vector2f position, float side_length)
	:parent(parent), position(position), side_length(side_length), mass(0.f), entity(nullptr) {
	
	shape.setPosition(position);
	shape.setSize(sf::Vector2f(side_length, side_length));
	shape.setFillColor(sf::Color::Transparent);
	shape.setOutlineThickness(1);
	shape.setOutlineColor(sf::Color(32, 94, 37, 255));

	north_west = nullptr;
	north_east = nullptr;
	south_west = nullptr;
	south_east = nullptr;
}

Node::~Node() {
	delete north_west;
	delete north_east;
	delete south_west;
	delete south_east;
}

QuadTree::QuadTree() {
	tree = nullptr;
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
	this->tree->update();
}

void QuadTree::build(CircGravEntity entities[], size_t size) {
	for (size_t i = 0; i < size; i++)
		this->tree->push(entities + i);
}

void QuadTree::draw(sf::RenderWindow *window) {
	if (tree != nullptr)
		tree->draw(window);
}