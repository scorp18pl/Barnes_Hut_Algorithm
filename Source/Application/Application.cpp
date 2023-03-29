#include <Application/Application.h>
#include <Simulation/Preset/SimulationPresetManager.h>
#include <imgui-SFML.h>
#include <imgui.h>
#include <imgui_stdlib.h>

static constexpr float DefaultMapSize = 1e8f;

Application& Application::GetApplication()
{
    static Application application;
    return application;
}

Application::Application()
    : m_window{ sf::VideoMode(1920, 1080), "Simulation", sf::Style::Fullscreen }
    , m_camera{ m_window }
    , m_simulation{ Uni::Math::BoundingBox2D{
          Uni::Math::Vector2f::CreateFromFloat(-DefaultMapSize / 2.0f),
          Uni::Math::Vector2f::CreateFromFloat(DefaultMapSize),
      } }
    , m_currentState{ State::Simulating }
{
    m_window.setFramerateLimit(60);
    m_camera.Zoom(1.0e-3f);

    if (!ImGui::SFML::Init(m_window))
    {
        // TODO
    }
}

bool Application::IsRunning() const
{
    return m_window.isOpen();
}

void Application::HandleMouseScroll(float delta)
{
    if (delta < 0.0f)
    {
        this->m_camera.Zoom(0.8f);
        return;
    }

    this->m_camera.Zoom(1.2f);
}

void Application::HandleEvents()
{
    sf::Event event;
    while (m_window.pollEvent(event))
    {
        ImGui::SFML::ProcessEvent(m_window, event);
        switch (event.type)
        {
        case sf::Event::Closed:
            m_window.close();
            break;
        case sf::Event::MouseWheelScrolled:
            HandleMouseScroll(event.mouseWheelScroll.delta);
            break;
        case sf::Event::MouseButtonPressed:
            break;
        case sf::Event::MouseButtonReleased:
            break;
        case sf::Event::KeyReleased:
            switch (event.key.code)
            {
            case sf::Keyboard::Q:
                //                FollowPrevious();
                break;
            case sf::Keyboard::E:
                //                FollowNext();
                break;
            case sf::Keyboard::Z:
                //                DisableFollow();
                break;
            default:
                break;
            }
        default:
            break;
        }
    }
}

void Application::Start()
{
    m_simulation.LoadPreset(SimulationPresetManager::Get().GetDefaultPreset());

    while (IsRunning())
    {
        Update();
        Render();
    }

    ImGui::SFML::Shutdown();
}

void Application::Update()
{
    HandleEvents();

    if (m_currentState == State::Simulating)
    {
        m_simulation.Update();
    }

    m_camera.Update();
    m_window.setView(m_camera.GetView());
}

void Application::OnImGuiUpdate()
{
    ImGui::SetNextWindowSize({ 600.0f, 800.0f });
    ImGui::Begin("Barnes Hut Simulation");
    ImGui::SetWindowFontScale(2.0f);

    if (ImGui::CollapsingHeader("Simulation Preset"))
    {
        ImGui::Indent(16.0f);
        OnImGuiPresetOptionsUpdate();
        ImGui::Unindent(16.0f);
    }

    if (m_currentState == State::Simulating)
    {
        m_simulation.OnImGuiUpdate();
    }
    else
    {
        // TODO
    }

    if (ImGui::Button("Quit"))
    {
        m_window.close();
    }

    ImGui::End();
}

void Application::OnImGuiPresetOptionsUpdate()
{
    const std::string CurrentPresetName = m_simulation.GetCurrentPresetName();
    SimulationPresetManager& presetManager = SimulationPresetManager::Get();
    const auto& availablePresets = presetManager.GetPresetNameList();

    ImGui::Text("Current Preset: %s", CurrentPresetName.c_str());

    if (ImGui::Button("Edit Current Preset"))
    {
        m_currentState = State::EditingPreset;
    }

    if (!presetManager.GetPreset(CurrentPresetName).m_isBuiltIn &&
        ImGui::Button("Delete Current Preset"))
    {
        presetManager.RemovePreset(CurrentPresetName);
        m_simulation.LoadPreset(presetManager.GetDefaultPreset());
    }

    ImGui::PushItemWidth(200.0f);
    if (ImGui::BeginCombo("Available Presets", CurrentPresetName.c_str()))
    {
        for (const std::string& name : availablePresets)
        {
            if (ImGui::Selectable(name.c_str()))
            {
                m_simulation.LoadPreset(presetManager.GetPreset(name));
            }
        }
        ImGui::EndCombo();
    }
    ImGui::PopItemWidth();

    if (ImGui::Button("Create Preset"))
    {
        m_currentState = State::EditingPreset;
    }
}

void Application::Render()
{
    m_window.clear();

    m_simulation.Draw(m_window);

    ImGui::SFML::Update(m_window, m_deltaClock.restart());
    OnImGuiUpdate();
    ImGui::SFML::Render(m_window);
    m_window.display();
}
