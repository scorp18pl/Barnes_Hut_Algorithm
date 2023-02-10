#include "Node.h"

const float Node::PHI = 0.5f;

void Node::setChildToNull(Quadrant quadrant) {
	this->subquadrants[(int)quadrant] = nullptr;
}

inline bool Node::isEmpty() {
	return !assignedEntity;
}

bool Node::isRoot() const {
    return !parent;
}

inline bool Node::hasNoChildren() {
	for (auto subquadrant : subquadrants)
		if (subquadrant)
			return false;
	
	return true;
}

bool Node::otherAreNull(Quadrant quadrant) {
	for (size_t i = 0; i < Node::CHILDREN_COUNT; i++)
		if (this->subquadrants[i] != nullptr && i != (size_t)quadrant)
			return false;

	return true;
}

bool Node::hasOnlyOneChild() {
	int children_count = 0;
	for (size_t i = 0; i < Node::CHILDREN_COUNT; i++)
		if (this->subquadrants[i] != nullptr)
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

bool Node::isInside(Entity *entity) {
	return !outOfReach(entity->getPosition());
}

bool Node::isEntityInside() {
	assert(!isEmpty());
	return isInside(assignedEntity);
}

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

Quadrant Node::selectQuadrant(sf::Vector2f position) {
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
		mass = assignedEntity->getMass();
		return;
	}

	float sum = 0.0f;
	for (size_t i = 0; i < Node::CHILDREN_COUNT; i++) {
		if (this->subquadrants[i]) {
			this->subquadrants[i]->updateMass();
			sum += this->subquadrants[i]->getMass();
		}
	}

	this->mass = sum;
}

void Node::updateCenterOfMass() {
	if (!isEmpty()) {
		this->center_of_mass = assignedEntity->getPosition();
		return;
	}

	for (size_t i = 0; i < Node::CHILDREN_COUNT; i++)
		if (this->subquadrants[i] != nullptr)
			this->subquadrants[i]->updateCenterOfMass();

	float center = 0.0f;
	//x coordinate
	for (size_t i = 0; i < Node::CHILDREN_COUNT; i++) {
		if (this->subquadrants[i] != nullptr) {
			float mass = this->subquadrants[i]->getMass();
			center += mass * this->subquadrants[i]->getCenterOfMass().x;
		}
	}

	center/= getMass();
	this->center_of_mass.x = center;

	//y coordinate
	center = 0.0f;
	for (size_t i = 0; i < Node::CHILDREN_COUNT; i++) {
		if (this->subquadrants[i] != nullptr) {
			float mass = this->subquadrants[i]->getMass();
			center += mass * this->subquadrants[i]->getCenterOfMass().y;
		}
	}

	center /= getMass();
	this->center_of_mass.y = center;
}

void Node::update() {
	if (!isEmpty()) {
		if (assignedEntity->isDisabled()) {
            assignedEntity = nullptr;
			this->parent->branchRemove(this->position, true);
		}
		else if (!isEntityInside() || !isSimplest()) {
			Entity *temp_entity = assignedEntity;
			moveUp(temp_entity, this->position, hasNoChildren());
		}
		return;
	}

	for (size_t i = 0; i < Node::CHILDREN_COUNT; i++)
		if (this->subquadrants[i] != nullptr)
			this->subquadrants[i]->update();
}

void Node::ensureQuadrantExists(Quadrant quadrant) {
    if (subquadrants[quadrant]) {
        return;
    }

    subquadrants[quadrant] = new Node(this, getChildPosition(quadrant), side_length / 2.0f);
}

void Node::push(Entity *entity) {
	assert(entity != nullptr);

	if (isEmpty() && hasNoChildren()) {
        assignedEntity = entity;
		return;
	}

	if (!this->isEmpty()) {
		Entity *tempEntity = assignedEntity;

		if (isEntityInside()) {
            assignedEntity = nullptr;
            const Quadrant tempQuadrant = selectQuadrant(tempEntity->getPosition());
            ensureQuadrantExists(tempQuadrant);
            subquadrants[tempQuadrant]->push(tempEntity);
		}
		else {
            assignedEntity = entity;
			parent->moveUp(tempEntity, this->position, false);
			return;
		}
	}

    const Quadrant quadrant = selectQuadrant(entity->getPosition());
    ensureQuadrantExists(quadrant);
	subquadrants[quadrant]->push(entity);
}

void Node::moveUp(Entity *entity, sf::Vector2f subquadrantPosition, bool removeSubquadrant) {
	if (removeSubquadrant) {
		Quadrant q = selectQuadrant(subquadrantPosition);
		QuadTree::stackPush(this->subquadrants[(int)q]);
		setChildToNull(q);
	}

	if (entity->isDisabled()) {
        return;
    }

	if (isInside(entity) && isSimplest()) {
		this->push(entity);
		return;
	}

    if (isRoot()) {
        entity->disable();
        return;
    }

	parent->moveUp(entity, this->position, hasNoChildren());
}

void Node::branchRemove(sf::Vector2f child_position, bool set_to_nullptr) {
	if (set_to_nullptr) {
		Quadrant q = selectQuadrant(child_position);
		QuadTree::stackPush(this->subquadrants[(int)q]);
		setChildToNull(q);
	}

	if (isSimplest())
		return;

	this->parent->branchRemove(this->position, hasNoChildren());
}

void Node::setOutlineThickness(float thickness) {
	this->shape.setOutlineThickness(thickness);

	for (size_t i = 0; i < Node::CHILDREN_COUNT; i++)
		if (this->subquadrants[i] != nullptr)
			this->subquadrants[i]->setOutlineThickness(thickness);
}

void Node::draw(sf::RenderWindow *window) {
	window->draw(this->shape);

	for (size_t i = 0; i < Node::CHILDREN_COUNT; i++)
		if (this->subquadrants[i] != nullptr)
			this->subquadrants[i]->draw(window);
}

void Node::calculateForce(Entity *entity, bool barnes_hut) {
	if (!isEmpty() && entity != assignedEntity) {
        entity->applyForce(entity->GForce(assignedEntity->getMass(), assignedEntity->getPosition()));
		return;
	}

	if (barnes_hut && isCloseEnough(entity)) {
        entity->applyForce(entity->GForce(this->mass, this->center_of_mass));
		return;
	}

	for (size_t i = 0; i < Node::CHILDREN_COUNT; i++)
		if (this->subquadrants[i] != nullptr)
			this->subquadrants[i]->calculateForce(entity, barnes_hut);
}

bool Node::isCloseEnough(Entity *entity) {
	return this->side_length < Node::PHI * entity->getDistance(this->center_of_mass);
}

int Node::countEntities() {
	int sum = 0;
	for (size_t i = 0; i < Node::CHILDREN_COUNT; i++)
		if (this->subquadrants[i] != nullptr)
			sum += this->subquadrants[i]->countEntities();

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
	assignedEntity(nullptr), center_of_mass(position) {
	
	this->shape.setPosition(position);
	this->shape.setSize(sf::Vector2f(side_length, side_length));
	this->shape.setFillColor(sf::Color::Transparent);
	this->shape.setOutlineThickness(1.0f);
	this->shape.setOutlineColor(sf::Color(32, 94, 37, 255));

	for (size_t i = 0; i < Node::CHILDREN_COUNT; i++)
		this->subquadrants[i] = nullptr;
}

Node::~Node() {
	for (size_t i = 0; i < Node::CHILDREN_COUNT; i++)
		delete this->subquadrants[i];
}

std::stack<Node *> QuadTree::stack;

QuadTree::QuadTree() 
	:barnes_hut(true), tree(nullptr) {
}

QuadTree::QuadTree(sf::Vector2f position, float side_length)
	: barnes_hut(true), tree(new Node(nullptr, position, side_length)) {
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

void QuadTree::calculateForces(Entity *entity) {
	this->tree->calculateForce(entity, this->barnes_hut);
}

void QuadTree::build(std::vector<Entity *> entities) {
	for (size_t i = 0; i < entities.size(); i++)
		this->tree->push(entities[i]);
}

void QuadTree::draw(sf::RenderWindow *window) {
	if (tree != nullptr) {
		tree->setOutlineThickness(window->getView().getSize().x / 1000.0f);
		tree->draw(window);
	}
}
