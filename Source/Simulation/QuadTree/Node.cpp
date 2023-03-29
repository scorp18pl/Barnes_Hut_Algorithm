#include "Node.h"
#include "QuadTree.h"
#include "Utils/Utils.h"

const float Node::PHI = 0.5f;

Node::Node(const Uni::Math::BoundingBox2D& boundingBox)
    : m_parentQuadrant{ nullptr }
    , m_parentSubquadrant{ NONE }
    , m_boundingBox{ boundingBox }
{
    InitializeLines();
}

Node::Node(Node* parent, Quadrant parentSubquadrant)
    : m_parentQuadrant{ parent }
    , m_parentSubquadrant{ parentSubquadrant }
    , m_boundingBox{ parent->GetSubquadrantBoundingBox(parentSubquadrant) }
{
    InitializeLines();
}

Node::~Node()
{
    for (Node* subquadrant : m_subquadrants)
    {
        delete subquadrant;
    }
}

bool Node::IsFarEnough(Entity* entity) const
{
    return std::max(
               m_boundingBox.GetDimensions().m_x,
               m_boundingBox.GetDimensions().m_y) <
        Node::PHI * (m_centerOfMass - entity->GetPosition()).GetLength();
}

float Node::GetMass() const
{
    return m_mass;
}

Uni::Math::Vector2f Node::GetCenterOfMass() const
{
    return m_centerOfMass;
}

void Node::Update()
{
    if (!IsEmpty())
    {
        const bool isInside = IsAssignedEntityInside();
        Entity* tempEntity = m_assignedEntity;

        if (IsRoot())
        {
            if (!isInside)
            {
                m_assignedEntity = nullptr;
                tempEntity->Disable();
            }
        }
        else if (!isInside || IsOnlyChild())
        {
            m_assignedEntity = nullptr;
            m_parentQuadrant->MoveUp(tempEntity, m_parentSubquadrant, true);
        }
        return;
    }

    for (Node* subquadrant : m_subquadrants)
    {
        if (subquadrant)
        {
            subquadrant->Update();
        }
    }
}

void Node::UpdateMass()
{
    if (!IsEmpty())
    {
        m_mass = m_assignedEntity->GetMass();
        return;
    }

    m_mass = 0.0f;
    for (Node* subquadrant : m_subquadrants)
    {
        if (subquadrant)
        {
            subquadrant->UpdateMass();
            m_mass += subquadrant->GetMass();
        }
    }
}

void Node::UpdateCenterOfMass()
{
    if (!IsEmpty())
    {
        m_centerOfMass = m_assignedEntity->GetPosition();
        return;
    }

    m_centerOfMass = Uni::Math::Vector2f::CreateZero();
    for (Node* subquadrant : m_subquadrants)
    {
        if (subquadrant)
        {
            subquadrant->UpdateCenterOfMass();
            m_centerOfMass +=
                subquadrant->GetCenterOfMass() * subquadrant->GetMass();
        }
    }

    m_centerOfMass /= m_mass;
}

void Node::EnsureSubquadrantExists(Quadrant subquadrant)
{
    if (m_subquadrants[subquadrant])
    {
        return;
    }

    m_subquadrants[subquadrant] = new Node(this, subquadrant);
}

void Node::Push(Entity* entity)
{
    assert(entity != nullptr);

    if (IsEmpty() && HasNoChildren())
    {
        m_assignedEntity = entity;
        return;
    }

    if (!IsEmpty())
    {
        Entity* assignedEntity = m_assignedEntity;
        if (IsAssignedEntityInside())
        {
            m_assignedEntity = nullptr;
            const Quadrant tempQuadrant =
                SelectSubquadrant(assignedEntity->GetPosition());
            EnsureSubquadrantExists(tempQuadrant);
            m_subquadrants[tempQuadrant]->Push(assignedEntity);
        }
        else
        {
            m_assignedEntity = entity;
            m_parentQuadrant->MoveUp(
                assignedEntity, m_parentSubquadrant, false);
            return;
        }
    }

    const Quadrant quadrant = SelectSubquadrant(entity->GetPosition());
    EnsureSubquadrantExists(quadrant);
    m_subquadrants[quadrant]->Push(entity);
}

void Node::MoveUp(Entity* entity, Quadrant subquadrant, bool removeSubquadrant)
{
    if (removeSubquadrant)
    {
        QuadTree::StackPush(m_subquadrants[subquadrant]);
        m_subquadrants[subquadrant] = nullptr;
    }

    if (!IsEntityInside(entity))
    {
        if (IsRoot())
        {
            // Let the simulation detect this and remove it.
            return;
        }

        m_parentQuadrant->MoveUp(entity, m_parentSubquadrant, HasNoChildren());
        return;
    }

    if (!IsOnlyChild() || IsRoot())
    {
        Push(entity);
        return;
    }

    m_parentQuadrant->MoveUp(entity, m_parentSubquadrant, HasNoChildren());
}

void Node::Draw(sf::RenderWindow& window) const
{
    window.draw(m_lines.data(), m_lines.size(), sf::Lines);

    for (Node* subquadrant : m_subquadrants)
    {
        if (subquadrant)
        {
            subquadrant->Draw(window);
        }
    }
}

void Node::ApplyGForceToEntity(Entity* entity, bool useBarnesHut)
{
    if (!IsEmpty() && entity != m_assignedEntity)
    {
        entity->ApplyForce(entity->CalculateGForce(
            m_assignedEntity->GetMass(), m_assignedEntity->GetPosition()));
        return;
    }

    if (useBarnesHut && IsFarEnough(entity))
    {
        entity->ApplyForce(entity->CalculateGForce(m_mass, m_centerOfMass));
        return;
    }

    for (Node* subquadrant : m_subquadrants)
    {
        if (subquadrant)
        {
            subquadrant->ApplyGForceToEntity(entity, useBarnesHut);
        }
    }
}

inline bool Node::IsEmpty() const
{
    return !m_assignedEntity;
}

bool Node::IsRoot() const
{
    return !m_parentQuadrant;
}

inline bool Node::HasNoChildren() const
{
    for (const Node* subquadrant : m_subquadrants)
    {
        if (subquadrant)
        {
            return false;
        }
    }

    return true;
}

bool Node::HasOnlyOneSubquadrant() const
{
    int subquadrantCount = 0;
    for (const Node* subquadrant : m_subquadrants)
    {
        if (subquadrant && ++subquadrantCount > 1)
        {
            return false;
        }
    }

    return subquadrantCount == 1;
}

bool Node::IsEntityInside(Entity* entity) const
{
    return m_boundingBox.IsPointWithinBounds(entity->GetPosition());
}

bool Node::IsAssignedEntityInside() const
{
    assert(!IsEmpty());
    return IsEntityInside(m_assignedEntity);
}

bool Node::IsOnlyChild() const
{
    if (!m_parentQuadrant)
    {
        return true;
    }

    return m_parentQuadrant->HasOnlyOneSubquadrant();
}

void Node::InitializeLines()
{
    static const sf::Color color = { 32, 94, 37, 255 };

    const sf::Vector2f UpperLeftCorner =
        Utils::CreateSfVectorFromUniVector(m_boundingBox.GetMinPoint());

    const sf::Vector2f UpperRightCorner = Utils::CreateSfVectorFromUniVector(
        m_boundingBox.GetMinPoint() +
        Uni::Math::Vector2f{ m_boundingBox.GetDimensions().m_x, 0.0f });

    const sf::Vector2f LowerLeftCorner = Utils::CreateSfVectorFromUniVector(
        m_boundingBox.GetMinPoint() +
        Uni::Math::Vector2f{ 0.0f, m_boundingBox.GetDimensions().m_y });

    const sf::Vector2f LowerRightCorner =
        Utils::CreateSfVectorFromUniVector(m_boundingBox.GetMaxPoint());

    m_lines = {
        sf::Vertex{ UpperLeftCorner, color },
        sf::Vertex{ UpperRightCorner, color },
        sf::Vertex{ UpperRightCorner, color },
        sf::Vertex{ LowerRightCorner, color },
        sf::Vertex{ LowerRightCorner, color },
        sf::Vertex{ LowerLeftCorner, color },
        sf::Vertex{ LowerLeftCorner, color },
        sf::Vertex{ UpperLeftCorner, color },
    };
}

Uni::Math::BoundingBox2D Node::GetSubquadrantBoundingBox(
    Quadrant subquadrant) const
{
    Uni::Math::Vector2f subquadrantMinPoint = m_boundingBox.GetMinPoint();
    Uni::Math::Vector2f subquadrantDimensions =
        m_boundingBox.GetDimensions() / 2.0f;

    switch (subquadrant)
    {
    case Quadrant::NORTH_EAST:
        subquadrantMinPoint.m_x += subquadrantDimensions.m_x;
        break;
    case Quadrant::SOUTH_WEST:
        subquadrantMinPoint.m_y += subquadrantDimensions.m_y;
        break;
    case Quadrant::SOUTH_EAST:
        subquadrantMinPoint += subquadrantDimensions;
        break;
    default:
        break;
    }

    return { subquadrantMinPoint, subquadrantDimensions };
}

Quadrant Node::SelectSubquadrant(Uni::Math::Vector2f position) const
{
    assert(m_boundingBox.GetDimensions() != Uni::Math::Vector2f::CreateZero());

    const Uni::Math::Vector2f middlePoint =
        m_boundingBox.GetMinPoint() + m_boundingBox.GetDimensions() / 2.0f;

    bool x_less, y_less;
    x_less = position.m_x < middlePoint.m_x;
    y_less = position.m_y < middlePoint.m_y;

    if (x_less)
    {
        if (y_less)
        {
            return Quadrant::NORTH_WEST;
        }
        else
        {
            return Quadrant::SOUTH_WEST;
        }
    }
    else if (y_less)
    {
        return Quadrant::NORTH_EAST;
    }

    return Quadrant::SOUTH_EAST;
}
