#include "Simulation.h"

#include "OmniMath.h"

Simulation::Simulation(size_t entity_count)
    : ENTITY_COUNT(entity_count)
    , m_drawTree(false)
    , m_followedIndex(0UL)
{
    m_quadTree = new QuadTree(m_map.GetStartingPosition(), m_map.GetSide());

    m_window = new sf::RenderWindow(
        sf::VideoMode(3840, 2160),
        "Simulation",
        sf::Style::Titlebar | sf::Style::Close);
    m_window->setFramerateLimit(60);
    m_camera = Camera(this->m_window);
    m_camera.Zoom(1.0e-3f);
}

Simulation::~Simulation()
{
    delete m_quadTree;
    delete m_window;
}

bool Simulation::IsRunning() const
{
    return m_window->isOpen();
}

void Simulation::Start()
{
    GenerateEntities();
    m_quadTree->Build(GetEntityPointers());

    while (IsRunning())
    {
        Update();
        Render();
    }
}

void Simulation::GenerateEntities()
{
    const float bigRadius = 1.0e5f;

    for (size_t i = 0; i < ENTITY_COUNT; ++i)
    {
        const float Div = 1e2f;
        const float V = 1e3f;

        float radius = MyRandom::CreateRandomFloat(1.0f, 1.0e3f);

        m_entities.emplace_back(new CircEntity(
            MyRandom::CreateRandomNormalVector2f() *
                MyRandom::CreateRandomFloat(bigRadius, m_map.GetSide() / Div),
            MyRandom::CreateRandomNormalVector2f() * V,
            radius * radius * radius,
            radius));
    }

    m_entities.emplace_back(new CircEntity(
        OMath::Vector2f::CreateZero(),
        OMath::Vector2f::CreateZero(),
        bigRadius * bigRadius * bigRadius,
        bigRadius));
}

void Simulation::PollEvents()
{
    sf::Event event;
    while (m_window->pollEvent(event))
    {
        switch (event.type)
        {
        case sf::Event::Closed:
            m_window->close();
            break;
        case sf::Event::KeyPressed:
            switch (event.key.code)
            {
            case sf::Keyboard::A:
                this->m_camera.Move(Direction::LEFT);
                break;
            case sf::Keyboard::W:
                this->m_camera.Move(Direction::UP);
                break;
            case sf::Keyboard::S:
                this->m_camera.Move(Direction::DOWN);
                break;
            case sf::Keyboard::D:
                this->m_camera.Move(Direction::RIGHT);
                break;
            case sf::Keyboard::P:
                this->m_camera.Zoom(1.2f);
                break;
            case sf::Keyboard::L:
                this->m_camera.Zoom(0.8f);
                break;
            default:
                break;
            }
            break;
        case sf::Event::KeyReleased:
            switch (event.key.code)
            {
            case sf::Keyboard::T:
                ToggleTreeDraw();
                break;
            case sf::Keyboard::F:
                ToggleKinematicsDebugDraw();
                break;
            case sf::Keyboard::B:
                ToggleBarnesHut();
                break;
            case sf::Keyboard::Q:
                FollowPrevious();
                break;
            case sf::Keyboard::E:
                FollowNext();
                break;
            case sf::Keyboard::Z:
                DisableFollow();
                break;
            case sf::Keyboard::G:
                ToggleTrackerDraw();
                break;
            case sf::Keyboard::C:
                ClearTrackers();
                break;
            case sf::Keyboard::I:
                ToggleTrackerLimit();
                break;
            default:
                break;
            }
        default:
            break;
        }
    }
}

void Simulation::Update()
{
    PollEvents();

    for (auto entity : m_entities)
    {
        entity->Update();
    }

    for (auto entity = m_entities.begin(); entity != m_entities.end(); ++entity)
    {
        if ((*entity)->IsDisabled())
        {
            if (m_camera.GetFollowed() == (*entity).get())
            {
                m_camera.DisableFollow();
                m_followedIndex = 0LU;
            }

            m_entities.erase(entity);
        }
    }

    m_camera.Update();
    m_window->setView(m_camera.GetView());

    m_quadTree->update();

    for (auto entity : m_entities)
    {
        entity->ClearAcceleration();
        m_quadTree->ApplyGForcesToEntity(entity.get());
    }
}

void Simulation::Render()
{
    m_window->clear();

    for (auto entity : m_entities)
    {
        entity->Draw(m_window);
    }

    if (m_drawTree)
    {
        m_quadTree->Draw(m_window);
    }

    m_window->display();
}

void Simulation::ToggleBarnesHut()
{
    m_quadTree->toggleBarnesHut();
    std::cout << "Toggled BarnesHut" << std::endl;
}

void Simulation::ToggleTreeDraw()
{
    m_drawTree = !m_drawTree;
}

void Simulation::ToggleKinematicsDebugDraw()
{
    Entity::ToggleDebugDraw();
}

void Simulation::ToggleTrackerDraw()
{
    Entity::ToggleTrackerDraw();
}

void Simulation::ToggleTrackerLimit()
{
    Tracker::ToggleLimit();
}

void Simulation::FollowPrevious()
{
    --m_followedIndex %= m_entities.size();
    m_camera.SetFollowed(m_entities[m_followedIndex].get());
}

void Simulation::FollowNext()
{
    ++m_followedIndex %= m_entities.size();
    m_camera.SetFollowed(m_entities[m_followedIndex].get());
}

void Simulation::DisableFollow()
{
    m_camera.DisableFollow();
}

void Simulation::ClearTrackers()
{
    for (auto& entity : m_entities)
    {
        entity->ClearTracker();
    }
}

std::vector<Entity*> Simulation::GetEntityPointers()
{
    std::vector<Entity*> entityPointers;
    entityPointers.reserve(m_entities.size());

    for (const auto& entity : m_entities)
    {
        entityPointers.push_back(entity.get());
    }

    return entityPointers;
}
