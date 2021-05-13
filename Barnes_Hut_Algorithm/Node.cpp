#include "Node.h"

QuadTree::QuadTree() {
	tree = nullptr;
}

QuadTree::QuadTree(sf::Vector2f position, float side_length) {
	std::cout << "QuadTree : " << position.x << " " << position.y << " - " << side_length << std::endl;
	this->tree = new Node(position, side_length);
}

QuadTree::~QuadTree() {
	delete tree;
}

Node *QuadTree::getTree() {
	return tree;
}

void QuadTree::build(CircGravEntity entities[], size_t size) {
	for (size_t i = 0; i < size; i++) {
		this->tree->push(entities + i);
		std::cout << entities[i].getPosition().x << " " << entities[i].getPosition().y << std::endl;
	}
}

void QuadTree::draw(sf::RenderWindow *window) {
	if (tree != nullptr)
		tree->draw(window);
}

bool Node::isEmpty() {
	std::cout << "isempty : " << (int)(entity == nullptr) << std::endl;
	return entity == nullptr;
}

bool Node::hasNoChildren() {
	std::cout << "hasnochildren : " << (int)(north_west == north_east && south_west == south_east) << std::endl;
	return north_west == north_east && south_west == south_east;
}

Quadrant Node::checkQuadrant(CircGravEntity* entity) {
	if (entity == nullptr)
		return Quadrant::ERROR;

	if (side_length == 0.f)
		exit(1);

	sf::Vector2f position;
	position = entity->getPosition();

	std::cout << "Check q: " << this->position.x << " " << this->position.y << std::endl;

	{
		bool out_of_reach;

		out_of_reach = position.x < this->position.x;
		out_of_reach = out_of_reach || position.y < this->position.y;

		out_of_reach = out_of_reach || position.x > this->position.x + side_length;
		out_of_reach = out_of_reach || position.y > this->position.y + side_length;

		if (out_of_reach)
			return Quadrant::ERROR;
	}

	bool x_less, y_less;

	x_less = position.x < this->position.x + side_length / 2.f;
	y_less = position.y < this->position.y + side_length / 2.f;

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

void Node::pushQ(CircGravEntity *entity, Quadrant q) {
	sf::Vector2f position = this->position;
	float side_length = this->side_length / 2.f;

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
		*n = new Node(position, side_length);

	(*n)->push(entity);
}

void Node::push(CircGravEntity *entity) {
	std::cout << "Pushing" << std::endl;

	if (entity == nullptr) {
		std::cout << "Invalid entity pointer at push method." << std::endl;
		return;
	}
	
	mass += entity->getMass();

	if (this->isEmpty() && this->hasNoChildren()) {
		this->entity = entity;
		return;
	}

	if (this->hasNoChildren()) {
		this->pushQ(this->entity, this->checkQuadrant(this->entity));
		this->entity = nullptr;
	}

	this->pushQ(entity, this->checkQuadrant(entity));
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

Node::Node(sf::Vector2f position, float side_length)
	:position(position), side_length(side_length), mass(0.f), entity(nullptr) {
	
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
	std::cout << "node deleted" << std::endl;

	delete north_west;
	delete north_east;
	delete south_west;
	delete south_east;
}
