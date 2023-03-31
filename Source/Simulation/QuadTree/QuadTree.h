#pragma once

#include <Simulation/QuadTree/Node.h>

class QuadTree
{
public:
    explicit QuadTree(const Uni::Math::BoundingBox2D& boundingBox);
    ~QuadTree();

    static void ToggleBarnesHut();
    static void ToggleDraw();
    static void StackPush(Node* node);

    void Update();
    void ApplyGForcesToEntity(CircEntity* entity);
    void Build(const std::vector<CircEntity*>& entities);
    void Draw(sf::RenderWindow& window) const;
    void Clear();

private:
    static bool BarnesHut, ShouldDraw;
    static std::stack<Node*> stack;

    Uni::Math::BoundingBox2D m_boundingBox;
    Node* m_tree;

    static void StackClear();
};