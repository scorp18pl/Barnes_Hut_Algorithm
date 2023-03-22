#include <Simulation.h>

#include <Universal/Math/Random.h>
#include <Universal/Profiling/TimeRegistry.h>
#include <Universal/Profiling/Timer.h>
#include <imgui-SFML.h>
#include <imgui.h>

Simulation::Simulation(size_t entity_count)
    : ENTITY_COUNT(entity_count)
    , m_drawTree(false)
    , m_followedIndex(0UL)
{
    constexpr float c_defaultMapSize = 1e8f;

    m_map = { Uni::Math::Vector2f::CreateFromFloat(-c_defaultMapSize / 2.0f),
           Uni::Math::Vector2f::CreateFromFloat(c_defaultMapSize) };

    m_quadTree = new QuadTree(m_map);

    m_window = new sf::RenderWindow(
        sf::VideoMode(1920, 1080), "Simulation", sf::Style::Fullscreen);
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

void Simulation::Start()
{
    GenerateEntities();
    m_quadTree->Build(GetEntityPointers());

    while (IsRunning())
    {
        Update();
        Render();
    }

    ImGui::SFML::Shutdown();
}

void Simulation::GenerateEntities()
{
    static constexpr float bigRadius = 1.0e5f;
    static constexpr float Div = 1e2f;
    static constexpr float V = 1e3f;

    for (size_t i = 0; i < ENTITY_COUNT; ++i)
    {
        const float radius =
            Uni::Math::Rand::CreateRandomUniformFloat(1.0f, 1.0e3f);

        m_entities.emplace_back(new CircEntity(
            Uni::Math::Vector2f::CreateRandomUnitVector() *
                Uni::Math::Rand::CreateRandomUniformFloat(
                    bigRadius, m_map.GetDimensions().m_x / Div),
            Uni::Math::Vector2f::CreateRandomUnitVector() * V,
            radius * radius * radius,
            radius));
    }

    m_entities.emplace_back(new CircEntity(
        Uni::Math::Vector2f::CreateZero(),
        Uni::Math::Vector2f::CreateZero(),
        bigRadius * bigRadius * bigRadius,
        bigRadius));
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
    Uni::Prof::Timer t("Update");
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
    Uni::Prof::Timer t("Render");
    m_window->clear();

    for (auto entity : m_entities)
    {
        entity->Draw(m_window);
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
        Uni::Prof::TimeRegistry::GetTimeRegistry().WriteToJsonFile(
            "/home/scorp/Documents/code/repos/universal/logs/");
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
