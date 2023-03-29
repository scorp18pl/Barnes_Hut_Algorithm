#pragma once

#include "Node.h"

class QuadTree
{
public:
    explicit QuadTree(const Uni::Math::BoundingBox2D& boundingBox);
    ~QuadTree();

    static void ToggleBarnesHut();
    static void ToggleDraw();
    static void StackPush(Node* node);

    void Update();
    void ApplyGForcesToEntity(Entity* entity);
    void Build(std::vector<Entity*> entities);
    void Draw(sf::RenderWindow& window) const;
    void Clear();

private:
    static bool BarnesHut, ShouldDraw;
    static std::stack<Node*> stack;

    Uni::Math::BoundingBox2D m_boundingBox;
    Node* m_tree;

    static void StackClear();
};