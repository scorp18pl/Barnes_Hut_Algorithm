#include "Node.h"

const float Node::PHI = 0.5f;

//Sets a quadrant child node to nullptr.
void Node::setChildToNull(Quadrant quadrant) {
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
bool Node::isInside(Entity *entity) {
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
		case Quadrant::NORTH_WEST:
			return return_v;
		case Quadrant::NORTH_EAST:
			return_v.x += this->side_length / 2.0f;
			return return_v;
		case Quadrant::SOUTH_WEST:
			return_v.y += this->side_length / 2.0f;
			return return_v;
		default:
			return_v.x += this->side_length / 2.0f;
			return_v.y += this->side_length / 2.0f;
			return return_v;
	}
}

//Returns the quadrant in which given position is
//situated. Considered quadrants' coordinate intervals
//are in the form [a, b) and [b, c] (for each).
Quadrant Node::checkQuadrant(sf::Vector2f position) {
	assert(this->side_length != 0.0f && !outOfReach(position));

	bool x_less, y_less;

	x_less = position.x < this->position.x + this->side_length / 2.0f;
	y_less = position.y < this->position.y + this->side_length / 2.0f;

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

void Node::updateMass() {
	if (!isEmpty()) {
		mass = this->entity->getMass();
		return;
	}

	float sum = 0.0f;
	for (size_t i = 0; i < Node::CHILDREN_COUNT; i++) {
		if (this->children[i] != nullptr) {
			this->children[i]->updateMass();
			sum += this->children[i]->getMass();
		}
	}

	this->mass = sum;
}

void Node::updateCenterOfMass() {
	if (!isEmpty()) {
		this->center_of_mass = this->entity->getPosition();
		return;
	}

	for (size_t i = 0; i < Node::CHILDREN_COUNT; i++)
		if (this->children[i] != nullptr)
			this->children[i]->updateCenterOfMass();

	float center = 0.0f;
	//x coordinate
	for (size_t i = 0; i < Node::CHILDREN_COUNT; i++) {
		if (this->children[i] != nullptr) {
			float mass = this->children[i]->getMass();
			center += mass * this->children[i]->getCenterOfMass().x;
		}
	}

	center/= getMass();
	this->center_of_mass.x = center;

	//y coordinate
	center = 0.0f;
	for (size_t i = 0; i < Node::CHILDREN_COUNT; i++) {
		if (this->children[i] != nullptr) {
			float mass = this->children[i]->getMass();
			center += mass * this->children[i]->getCenterOfMass().y;
		}
	}

	center /= getMass();
	this->center_of_mass.y = center;
}

void Node::update() {
	if (!isEmpty()) {
		if (this->entity->isDisabled()) {
			delete this->entity;
			this->parent->branchRemove(this->position, true);
		}
		else if (!isEntityInside() || !isSimplest()) {
			Entity *temp_entity = this->entity;
			moveUp(temp_entity, this->position, hasNoChildren());
		}
		return;
	}

	for (size_t i = 0; i < Node::CHILDREN_COUNT; i++)
		if (this->children[i] != nullptr)
			this->children[i]->update();
}

void Node::pushQ(Entity *entity, Quadrant q) {
	if (this->children[(int)q] == nullptr) {
		sf::Vector2f position = this->getChildPosition(q);
		float side_length = this->side_length / 2.0f;

		this->children[(int)q] = new Node(this, position, side_length);
	}

	this->children[(int)q]->push(entity);
}

void Node::push(Entity *entity) {
	assert(entity != nullptr && isInside(entity));

	if (this->isEmpty() && this->hasNoChildren()) {
		this->entity = entity;
		return;
	}

	if (!this->isEmpty()) {
		Entity *temp_entity = this->entity;

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

//Recursively moves the entity up the quadtree until it
//it is contained within the node .
void Node::moveUp(Entity *entity, sf::Vector2f child_position, bool set_to_nullptr) {
	if (set_to_nullptr) {
		Quadrant q = checkQuadrant(child_position);
		QuadTree::stackPush(this->children[(int)q]);
		setChildToNull(q);
	}

	if (entity->isDisabled())
		return;

	if (isInside(entity) && isSimplest()) {
		this->push(entity);
		return;
	}

	parent->moveUp(entity, this->position, hasNoChildren());
}

void Node::branchRemove(sf::Vector2f child_position, bool set_to_nullptr) {
	if (set_to_nullptr) {
		Quadrant q = checkQuadrant(child_position);
		QuadTree::stackPush(this->children[(int)q]);
		setChildToNull(q);
	}

	if (isSimplest())
		return;

	this->parent->branchRemove(this->position, hasNoChildren());
}

void Node::setOutlineThickness(float thickness) {
	this->shape.setOutlineThickness(thickness);

	for (size_t i = 0; i < Node::CHILDREN_COUNT; i++)
		if (this->children[i] != nullptr)
			this->children[i]->setOutlineThickness(thickness);
}

void Node::draw(sf::RenderWindow *window) {
	window->draw(this->shape);

	for (size_t i = 0; i < Node::CHILDREN_COUNT; i++)
		if (this->children[i] != nullptr)
			this->children[i]->draw(window);
}

void Node::calculateForce(Entity *entity, bool barnes_hut) {
	if (!isEmpty() && entity != this->entity) {
		entity->accelerate(entity->GForce(this->entity->getMass(), this->entity->getPosition()));
		return;
	}

	if (barnes_hut && isCloseEnough(entity)) {
		entity->accelerate(entity->GForce(this->mass, this->center_of_mass));
		return;
	}

	for (size_t i = 0; i < Node::CHILDREN_COUNT; i++)
		if (this->children[i] != nullptr)
			this->children[i]->calculateForce(entity, barnes_hut);
}

bool Node::isCloseEnough(Entity *entity) {
	return this->side_length < Node::PHI * entity->getDistance(this->center_of_mass);
}

int Node::countEntities() {
	int sum = 0;
	for (size_t i = 0; i < Node::CHILDREN_COUNT; i++)
		if (this->children[i] != nullptr)
			sum += this->children[i]->countEntities();

	if (!this->isEmpty())
		sum++;

	return sum;
}

float Node::getMass() {
	return this->mass;
}

sf::Vector2f Node::getCenterOfMass() {
	return this->center_of_mass;
}

Node::Node(Node *parent, sf::Vector2f position, float side_length)
	:parent(parent), position(position), side_length(side_length), mass(0.0f),
	entity(nullptr), center_of_mass(position) {
	
	this->shape.setPosition(position);
	this->shape.setSize(sf::Vector2f(side_length, side_length));
	this->shape.setFillColor(sf::Color::Transparent);
	this->shape.setOutlineThickness(1.0f);
	this->shape.setOutlineColor(sf::Color(32, 94, 37, 255));

	for (size_t i = 0; i < Node::CHILDREN_COUNT; i++)
		this->children[i] = nullptr;
}

Node::~Node() {
	for (size_t i = 0; i < Node::CHILDREN_COUNT; i++)
		delete this->children[i];
}

std::stack<Node *> QuadTree::stack;

QuadTree::QuadTree() 
	:barnes_hut(true), tree(nullptr) {
}

QuadTree::QuadTree(Node *parent, sf::Vector2f position, float side_length)
	: barnes_hut(true), tree(new Node(parent, position, side_length)) {
}

QuadTree::~QuadTree() {
	delete tree;
}

void QuadTree::stackPush(Node * node) {
	QuadTree::stack.push(node);
}

void QuadTree::stackClear() {
	while (!QuadTree::stack.empty()) {
		delete QuadTree::stack.top();
		QuadTree::stack.pop();
	}
}

Node *QuadTree::getTree() {
	return tree;
}

void QuadTree::update() {
	this->tree->update();
	stackClear();

	this->tree->updateMass();
	this->tree->updateCenterOfMass();
}

void QuadTree::toggleBarnesHut() {
	this->barnes_hut = !this->barnes_hut;
}

//Recursively calculates forces for all
//bodies.
void QuadTree::calculateForces(Entity *entity) {
	this->tree->calculateForce(entity, this->barnes_hut);
}

void QuadTree::build(std::vector<CircEntity *> entities) {
	for (size_t i = 0; i < entities.size(); i++)
		this->tree->push(entities[i]);
}

void QuadTree::draw(sf::RenderWindow *window) {
	if (tree != nullptr) {
		tree->setOutlineThickness(window->getView().getSize().x / 1000.0f);
		tree->draw(window);
	}
}
