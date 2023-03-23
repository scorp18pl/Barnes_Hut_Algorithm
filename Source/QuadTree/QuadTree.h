#pragma once

#include <QuadTree/Node.h>

class QuadTree
{
public:
    QuadTree();
    QuadTree(const Uni::Math::BoundingBox2D& boundingBox);
    ~QuadTree();

    static void StackPush(Node* node);

    Node* getTree();

    void update();
    void toggleBarnesHut();
    void ApplyGForcesToEntity(Entity* entity);
    void Build(std::vector<Entity*> entities);
    void Draw(sf::RenderWindow* window);

private:
    bool m_barnes_hut;
    static std::stack<Node*> stack;
    Node* m_tree;

    static void StackClear();
};