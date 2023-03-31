#pragma once

#include <Simulation/Entity/CircEntity.h>
#include <Universal/Math/BoundingBox2D.h>
#include <array>
#include <cassert>
#include <iostream>
#include <stack>

enum Quadrant : int
{
    NONE = -1,
    NORTH_WEST = 0,
    NORTH_EAST = 1,
    SOUTH_WEST = 2,
    SOUTH_EAST = 3,
};

class Node
{
public:
    static constexpr size_t SUBQUADRANT_COUNT = 4;
    static const float PHI;

    explicit Node(const Uni::Math::BoundingBox2D& boundingBox);
    Node(Node* parent, Quadrant parentSubquadrant);
    ~Node();

    void Update();
    void UpdateMass();
    void UpdateCenterOfMass();
    void Push(CircEntity* entity);

    //! Recursively moves the entity up the quadtree until it
    //! it is contained within the node .
    //! @param removeChild Should the child node be removed?
    void MoveUp(CircEntity* entity, Quadrant subquadrant, bool removeChild);

    void Draw(sf::RenderWindow& window) const;

    //! Recursively calculates forces for all
    //! bodies.
    void ApplyGForceToEntity(CircEntity* entity, bool useBarnesHut);

private:
    Uni::Math::BoundingBox2D m_boundingBox;
    float m_mass{ 0.0f };
    Uni::Math::Vector2f m_centerOfMass{ Uni::Math::Vector2f::CreateZero() };

    CircEntity* m_assignedEntity{ nullptr };

    Node* m_parent;
    Quadrant m_subquadrant;
    std::array<Node*, SUBQUADRANT_COUNT> m_children{ { nullptr } };

    std::array<sf::Vertex, 8> m_lines;

    //! The node is empty when it has no assigned entity.
    [[nodiscard]] bool IsEmpty() const;
    [[nodiscard]] bool IsRoot() const;
    //! Returns whether or not a node has children.
    [[nodiscard]] bool HasNoChildren() const;
    //! Returns whether or not the node has only a one child.
    [[nodiscard]] bool HasOnlyOneSubquadrant() const;
    //! Returns whether or not given entity is contained
    //! within the bounds of the node.
    bool IsEntityInside(CircEntity* entity) const;
    //! Returns whether or not entity assigned to the node
    //! is contained within the bounds of the node.
    [[nodiscard]] bool IsAssignedEntityInside() const;
    //! Returns whether or not a leaf node should be deleted
    //! and the entity moved up the tree.
    [[nodiscard]] bool IsOnlyChild() const;

    void InitializeLines();
    void EnsureSubquadrantExists(Quadrant subquadrant);
    bool IsFarEnough(CircEntity* entity) const;

    [[nodiscard]] Uni::Math::BoundingBox2D GetSubquadrantBoundingBox(Quadrant subquadrant) const;
    //! Returns the quadrant in which given m_position is
    //! situated. Considered quadrants' coordinate intervals
    //! are in the form [a, b) and [b, c] (for each).
    [[nodiscard]] Quadrant SelectSubquadrant(Uni::Math::Vector2f position) const;
};
