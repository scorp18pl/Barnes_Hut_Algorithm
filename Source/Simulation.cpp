#include <Simulation.h>
#include <SimulationPreset/SimulationPresetSerializer.h>
#include <Universal/Math/Random.h>
#include <Universal/Profiling/TimeRegistry.h>
#include <Universal/Profiling/Timer.h>
#include <imgui-SFML.h>
#include <imgui.h>

const Uni::Math::BoundingBox2D Simulation::Map{
    Uni::Math::Vector2f::CreateFromFloat(-DefaultMapSize / 2.0f),
    Uni::Math::Vector2f::CreateFromFloat(DefaultMapSize)
};

Simulation& Simulation::GetSimulation()
{
    static Simulation simulation;
    return simulation;
}

Simulation::Simulation()
    : m_currentSimulationPreset{ GenerateDefaultSimulationPreset() }
    , m_drawTree{ false }
    , m_followedIndex{ 0UL }
{
    m_quadTree = new QuadTree(Map);
#ifndef NDEBUG
    m_window = new sf::RenderWindow(sf::VideoMode(1920, 1080), "Simulation");
#else
    m_window = new sf::RenderWindow(
        sf::VideoMode(1920, 1080), "Simulation", sf::Style::Fullscreen);
#endif
    m_window->setFramerateLimit(60);
    m_camera = Camera(this->m_window);
    m_camera.Zoom(1.0e-3f);

    if (!ImGui::SFML::Init(*m_window))
    {
        return;
    }
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

const SimulationPreset& Simulation::GetCurrentSimulationPreset() const
{
    return m_currentSimulationPreset;
}

void Simulation::Start()
{
    GenerateDefaultSimulationPreset();
    m_quadTree->Build(GetEntityPointers());

    while (IsRunning())
    {
        Update();
        Render();
    }

    ImGui::SFML::Shutdown();
}

SimulationPreset Simulation::GenerateDefaultSimulationPreset()
{
    static constexpr size_t EntityCount = 3000LU;
    static constexpr float Div = 1.0e2f;
    static constexpr float V = 1.0e3f;
    static constexpr float BigRadius = 1.0e5f;

    SimulationPreset defaultSimulationPreset;

    defaultSimulationPreset.m_entities.reserve(EntityCount);
    defaultSimulationPreset.m_entities.emplace_back(
        Uni::Math::Vector2f::CreateZero(),
        Uni::Math::Vector2f::CreateZero(),
        BigRadius);

    for (size_t i = 1LU; i < EntityCount; ++i)
    {
        const Uni::Math::Vector2f Position =
            Uni::Math::Vector2f::CreateRandomUnitVector() *
            Uni::Math::Rand::CreateRandomUniformFloat(
                BigRadius, Map.GetDimensions().m_x / Div);

        const Uni::Math::Vector2f Velocity =
            Uni::Math::Vector2f::CreateRandomUnitVector() * V;

        const float Radius =
            Uni::Math::Rand::CreateRandomUniformFloat(1.0f, 1.0e3f);

        defaultSimulationPreset.m_entities.emplace_back(
            Position, Velocity, Radius);
    }

    SimulationPresetSerializer::SaveSimulationPreset(defaultSimulationPreset);
    return defaultSimulationPreset;
}

void Simulation::PollEvents()
{
    sf::Event event;
    while (m_window->pollEvent(event))
    {
        ImGui::SFML::ProcessEvent(*m_window, event);
        switch (event.type)
        {
        case sf::Event::Closed:
            m_window->close();
            break;
        case sf::Event::MouseWheelScrolled:
            HandleMouseScroll(event.mouseWheelScroll.delta);
            break;
        case sf::Event::KeyReleased:
            switch (event.key.code)
            {
            case sf::Keyboard::Q:
                FollowPrevious();
                break;
            case sf::Keyboard::E:
                FollowNext();
                break;
            case sf::Keyboard::Z:
                DisableFollow();
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

    for (auto& entity : m_currentSimulationPreset.m_entities)
    {
        if (!entity.IsDisabled())
        {
            entity.Update();
        }
    }

    m_quadTree->update();

    m_currentSimulationPreset.m_entities.erase(
        std::remove_if(
            m_currentSimulationPreset.m_entities.begin(),
            m_currentSimulationPreset.m_entities.end(),
            [this](CircEntity& entity)
            {
                if (m_camera.GetFollowed() == &entity)
                {
                    m_camera.DisableFollow();
                    m_followedIndex = 0LU;
                }

                return !Map.IsPointWithinBounds(entity.GetPosition());
            }),
        m_currentSimulationPreset.m_entities.end());

    m_camera.Update();
    m_window->setView(m_camera.GetView());

    for (auto& entity : m_currentSimulationPreset.m_entities)
    {
        entity.ClearAcceleration();
        m_quadTree->ApplyGForcesToEntity(&entity);
    }
}

void Simulation::Render()
{
    m_window->clear();

    for (auto& entity : m_currentSimulationPreset.m_entities)
    {
        entity.Draw(m_window);
    }

    if (m_drawTree)
    {
        m_quadTree->Draw(m_window);
    }

    ImGui::SFML::Update(*m_window, m_deltaClock.restart());

    OnImGuiUpdate();

    ImGui::SFML::Render(*m_window);
    m_window->display();
}

void Simulation::OnImGuiUpdate()
{
    ImGui::SetNextWindowSize({ 400.0f, 800.0f });
    ImGui::Begin("Barnes Hut Simulation");
    ImGui::SetWindowFontScale(2.0f);

    if (ImGui::CollapsingHeader("Debug options"))
    {
        ImGui::Indent(16.0f);
        if (ImGui::Button("Toggle Quadtree visuals"))
        {
            ToggleTreeDraw();
        }

        if (ImGui::Button("Toggle Vector visuals"))
        {
            ToggleKinematicsDebugDraw();
        }

        if (ImGui::Button("Algorithm"))
        {
            ToggleBarnesHut();
        }
        ImGui::Unindent(16.0f);
    }

    if (ImGui::CollapsingHeader("Tracker options"))
    {
        ImGui::Indent(16.0f);
        if (ImGui::Button("Toggle Visuals"))
        {
            ToggleTrackerDraw();
        }

        if (ImGui::Button("Toggle Limit"))
        {
            ToggleTrackerLimit();
        }

        if (ImGui::Button("Clear"))
        {
            ClearTrackers();
        }
        ImGui::Unindent(16.0f);
    }

    if (ImGui::Button("Quit"))
    {
        m_window->close();
    }

    ImGui::End();
}

void Simulation::HandleMouseScroll(float delta)
{
    if (delta < 0.0f)
    {
        this->m_camera.Zoom(0.8f);
        return;
    }

    this->m_camera.Zoom(1.2f);
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
    --m_followedIndex %= m_currentSimulationPreset.m_entities.size();
    m_camera.SetFollowed(
        &m_currentSimulationPreset.m_entities[m_followedIndex]);
}

void Simulation::FollowNext()
{
    ++m_followedIndex %= m_currentSimulationPreset.m_entities.size();
    m_camera.SetFollowed(
        &m_currentSimulationPreset.m_entities[m_followedIndex]);
}

void Simulation::DisableFollow()
{
    m_camera.DisableFollow();
}

void Simulation::ClearTrackers()
{
    for (auto& entity : m_currentSimulationPreset.m_entities)
    {
        entity.ClearTracker();
    }
}

std::vector<Entity*> Simulation::GetEntityPointers()
{
    std::vector<Entity*> entityPointers;
    entityPointers.reserve(m_currentSimulationPreset.m_entities.size());

    for (auto& entity : m_currentSimulationPreset.m_entities)
    {
        entityPointers.push_back(&entity);
    }

    return entityPointers;
}
