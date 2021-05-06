#pragma once
#include "Entity.h"

class QuadTree {
public:
	//Constructors and Destructors
	QuadTree();
	~QuadTree();

	void build(GravEntity[] entities);
};

class Node {
};

