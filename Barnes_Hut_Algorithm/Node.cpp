#include "Node.h"

#include <Utils.h>

const float Node::PHI = 0.5f;

Node::Node(Uni::Math::Vector2f position, float sideLength)
    : m_parentQuadrant{ nullptr }
    , m_parentSubquadrant{ NONE }
    , m_position{ position }
    , m_sideLength{ sideLength }
{
    m_shape.setPosition(Utils::CreateSfVectorFromUniVector(position));
    m_shape.setSize(Utils::CreateSfVectorFromUniVector(
        Uni::Math::Vector2f::CreateFromFloat(sideLength)));
    m_shape.setFillColor(sf::Color::Transparent);
    m_shape.setOutlineThickness(1.0f);
    m_shape.setOutlineColor(sf::Color(32, 94, 37, 255));
}

Node::Node(Node* parent, Quadrant parentSubquadrant)
    : m_parentQuadrant{ parent }
    , m_parentSubquadrant{ parentSubquadrant }
    , m_position{ parent->GetSubquadrantPosition(parentSubquadrant) }
    , m_sideLength{ parent->m_sideLength / 2.0f }
{
    m_shape.setPosition(Utils::CreateSfVectorFromUniVector(m_position));
    m_shape.setSize(
        Utils::CreateSfVectorFromUniVector(Uni::Math::Vector2f::CreateFromFloat(m_sideLength)));
    m_shape.setFillColor(sf::Color::Transparent);
    m_shape.setOutlineThickness(1.0f);
    m_shape.setOutlineColor(sf::Color(32, 94, 37, 255));
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
    return m_sideLength <
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
            entity->Disable();
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

void Node::SetOutlineThickness(float thickness)
{
    m_shape.setOutlineThickness(thickness);

    for (Node* subquadrant : m_subquadrants)
    {
        if (subquadrant)
        {
            subquadrant->SetOutlineThickness(thickness);
        }
    }
}

void Node::Draw(sf::RenderWindow* window)
{
    window->draw(m_shape);

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

bool Node::IsPositionOutOfBounds(Uni::Math::Vector2f position) const
{
    bool outOfBounds = position.m_x < m_position.m_x;
    outOfBounds = outOfBounds || position.m_y < m_position.m_y;

    outOfBounds = outOfBounds || position.m_x >= m_position.m_x + m_sideLength;
    outOfBounds = outOfBounds || position.m_y >= m_position.m_y + m_sideLength;

    return outOfBounds;
}

bool Node::IsEntityInside(Entity* entity) const
{
    return !IsPositionOutOfBounds(entity->GetPosition());
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

Uni::Math::Vector2f Node::GetSubquadrantPosition(Quadrant subquadrant) const
{
    Uni::Math::Vector2f subquadrantPosition = m_position;

    switch (subquadrant)
    {
    case Quadrant::NORTH_WEST:
        return subquadrantPosition;
    case Quadrant::NORTH_EAST:
        subquadrantPosition.m_x += m_sideLength / 2.0f;
        return subquadrantPosition;
    case Quadrant::SOUTH_WEST:
        subquadrantPosition.m_y += m_sideLength / 2.0f;
        return subquadrantPosition;
    default:
        subquadrantPosition +=
            Uni::Math::Vector2f::CreateFromFloat(m_sideLength / 2.0f);
        return subquadrantPosition;
    }
}

Quadrant Node::SelectSubquadrant(Uni::Math::Vector2f position) const
{
    assert(m_sideLength != 0.0f);

    bool x_less, y_less;
    x_less = position.m_x < m_position.m_x + (m_sideLength / 2.0f);
    y_less = position.m_y < m_position.m_y + (m_sideLength / 2.0f);

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

std::stack<Node*> QuadTree::stack;

QuadTree::QuadTree()
    : m_barnes_hut{ true }
    , m_tree{ nullptr }
{
}

QuadTree::QuadTree(Uni::Math::Vector2f position, float side_length)
    : m_barnes_hut{ true }
    , m_tree{ new Node(position, side_length) }
{
}

QuadTree::~QuadTree()
{
    delete m_tree;
}

void QuadTree::StackPush(Node* node)
{
    QuadTree::stack.push(node);
}

Node* QuadTree::getTree()
{
    return m_tree;
}

void QuadTree::update()
{
    m_tree->Update();
    StackClear();

    m_tree->UpdateMass();
    m_tree->UpdateCenterOfMass();
}

void QuadTree::toggleBarnesHut()
{
    m_barnes_hut = !m_barnes_hut;
}

void QuadTree::ApplyGForcesToEntity(Entity* entity)
{
    m_tree->ApplyGForceToEntity(entity, m_barnes_hut);
}

void QuadTree::Build(std::vector<Entity*> entities)
{
    for (Entity* entity : entities)
    {
        m_tree->Push(entity);
    }
}

void QuadTree::Draw(sf::RenderWindow* window)
{
    if (m_tree)
    {
        m_tree->SetOutlineThickness(window->getView().getSize().x / 1000.0f);
        m_tree->Draw(window);
    }
}

void QuadTree::StackClear()
{
    while (!QuadTree::stack.empty())
    {
        delete QuadTree::stack.top();
        QuadTree::stack.pop();
    }
}