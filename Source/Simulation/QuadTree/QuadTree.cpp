#include "QuadTree.h"

std::stack<Node*> QuadTree::stack;
bool QuadTree::BarnesHut = true;
bool QuadTree::ShouldDraw = false;

QuadTree::QuadTree(const Uni::Math::BoundingBox2D& boundingBox)
    : m_tree{ new Node(boundingBox) }
    , m_boundingBox{ boundingBox }
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

void QuadTree::Update()
{
    m_tree->Update();
    StackClear();

    m_tree->UpdateMass();
    m_tree->UpdateCenterOfMass();
}

void QuadTree::ToggleBarnesHut()
{
    BarnesHut = !BarnesHut;
}

void QuadTree::ToggleDraw()
{
    ShouldDraw = !ShouldDraw;
}

void QuadTree::ApplyGForcesToEntity(Entity* entity)
{
    m_tree->ApplyGForceToEntity(entity, BarnesHut);
}

void QuadTree::Build(std::vector<Entity*> entities)
{
    for (Entity* entity : entities)
    {
        m_tree->Push(entity);
    }
}

void QuadTree::Draw(sf::RenderWindow& window) const
{
    if (ShouldDraw && m_tree)
    {
        m_tree->Draw(window);
    }
}

void QuadTree::Clear()
{
    delete m_tree;
    m_tree = new Node{ m_boundingBox };
}

void QuadTree::StackClear()
{
    while (!QuadTree::stack.empty())
    {
        delete QuadTree::stack.top();
        QuadTree::stack.pop();
    }
}
