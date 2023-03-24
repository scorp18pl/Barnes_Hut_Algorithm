#pragma once

#include <Entity/Entity.h>
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

    Node(const Uni::Math::BoundingBox2D& boundingBox);
    Node(Node* parent, Quadrant parentSubquadrant);
    ~Node();

    bool IsFarEnough(Entity* entity) const;
    float GetMass() const;
    Uni::Math::Vector2f GetCenterOfMass() const;

    void Update();
    void UpdateMass();
    void UpdateCenterOfMass();
    void EnsureSubquadrantExists(Quadrant subquadrant);
    void Push(Entity* entity);

    //! Recursively moves the entity up the quadtree until it
    //! it is contained within the node .
    //! @param removeSubquadrant Should the subquadrant be removed?
    void MoveUp(Entity* entity, Quadrant subquadrant, bool removeSubquadrant);

    void SetOutlineThickness(float thickness);
    void Draw(sf::RenderWindow* window);

    //! Recursively calculates forces for all
    //! bodies.
    void ApplyGForceToEntity(Entity* entity, bool useBarnesHut);

private:
    Uni::Math::BoundingBox2D m_boundingBox;
    float m_mass{ 0.0f };
    Uni::Math::Vector2f m_centerOfMass{ Uni::Math::Vector2f::CreateZero() };

    Entity* m_assignedEntity{ nullptr };

    Node* m_parentQuadrant;
    Quadrant m_parentSubquadrant;
    std::array<Node*, SUBQUADRANT_COUNT> m_subquadrants{ { nullptr } };

    sf::RectangleShape m_shape;

    //! The node is empty when it has no assigned entity.
    bool IsEmpty() const;
    bool IsRoot() const;
    //! Returns whether or not a node has m_subquadrants.
    bool HasNoChildren() const;
    //! Returns whether or not the node has only a one child.
    bool HasOnlyOneSubquadrant() const;
    //! Returns whether or not given entity is contained
    //! within the bounds of the node.
    bool IsEntityInside(Entity* entity) const;
    //! Returns whether or not entity assigned to the node
    //! is contained within the bounds of the node.
    bool IsAssignedEntityInside() const;
    //! Returns whether or not a leaf node should be deleted
    //! and the entity moved up the m_tree.
    bool IsOnlyChild() const;

    Uni::Math::BoundingBox2D GetSubquadrantBoundingBox(Quadrant subquadrant) const;
    //! Returns the quadrant in which given m_position is
    //! situated. Considered quadrants' coordinate intervals
    //! are in the form [a, b) and [b, c] (for each).
    Quadrant SelectSubquadrant(Uni::Math::Vector2f position) const;
};
