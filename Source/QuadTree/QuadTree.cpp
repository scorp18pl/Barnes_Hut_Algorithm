#include <QuadTree/QuadTree.h>

std::stack<Node*> QuadTree::stack;

QuadTree::QuadTree()
    : m_barnes_hut{ true }
    , m_tree{ nullptr }
{
}

QuadTree::QuadTree(const Uni::Math::BoundingBox2D& boundingBox)
    : m_barnes_hut{ true }
    , m_tree{ new Node(boundingBox) }
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
