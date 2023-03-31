#include <Application/Application.h>
#include <Simulation/Preset/SimulationPresetManager.h>
#include <imgui-SFML.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_stdlib.h>

static constexpr float DefaultMapSize = 1e8f;

Application& Application::GetApplication()
{
    static Application application;
    return application;
}

Application::Application()
    : m_window{ sf::VideoMode(1920, 1080), "Simulation", sf::Style::Fullscreen }
    , m_simulation{ Uni::Math::BoundingBox2D{
          Uni::Math::Vector2f::CreateFromFloat(-DefaultMapSize / 2.0f),
          Uni::Math::Vector2f::CreateFromFloat(DefaultMapSize),
      } }
    , m_currentState{ State::Simulating }
{
    m_window.setFramerateLimit(60);
    Camera& camera = Camera::Get();
    camera.SetView(m_window.getView());
    camera.Zoom(1.0e-3f);

    if (!ImGui::SFML::Init(m_window))
    {
        // Too bad
    }
}

bool Application::IsRunning() const
{
    return m_window.isOpen();
}

void Application::HandleMouseScroll(float delta)
{
    Camera& camera = Camera::Get();
    if (delta < 0.0f)
    {
        camera.Zoom(0.8f);
        return;
    }

    camera.Zoom(1.2f);
}

void Application::HandleKeyInputs()
{
    static constexpr float Force = 1e-3f;
    Uni::Math::Vector2f input = Uni::Math::Vector2f::CreateZero();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        input += Uni::Math::Vector2f{ 0.0f, -1.0f };
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        input += Uni::Math::Vector2f{ 0.0f, 1.0f };
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        input += Uni::Math::Vector2f{ -1.0f, 0.0f };
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        input += Uni::Math::Vector2f{ 1.0f, 0.0f };
    }

    Camera& camera = Camera::Get();
    float force = Force * camera.GetView().getSize().x;
    camera.ApplyForce(input.GetNormalized() * force);
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
            if (!ImGui::GetIO().WantCaptureMouse)
            {
                HandleMouseScroll(event.mouseWheelScroll.delta);
            }
            break;
        default:
            break;
        }
    }

    if (!ImGui::GetIO().WantCaptureKeyboard)
    {
        HandleKeyInputs();
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

    if (m_currentState == State::Simulating && !m_isSimulationPaused)
    {
        m_simulation.Update();
    }

    Camera& camera = Camera::Get();
    camera.Update();
    m_window.setView(camera.GetView());
}

void Application::OnImGuiUpdate()
{
    ImGui::SetNextWindowSize({ 600.0f, 800.0f });
    ImGui::Begin("Barnes Hut Simulation");
    ImGui::SetWindowFontScale(2.0f);

    if (m_currentState == State::Simulating)
    {
        if (ImGui::CollapsingHeader("Simulation Preset"))
        {
            ImGui::Indent(16.0f);
            OnImGuiPresetOptionsUpdate();
            ImGui::Unindent(16.0f);
        }

        m_simulation.OnImGuiUpdate();

        if (ImGui::Button(m_isSimulationPaused ? "Resume" : "Pause"))
        {
            m_isSimulationPaused = !m_isSimulationPaused;
        }
        ImGui::SameLine();
        if (ImGui::Button("Quit"))
        {
            m_window.close();
        }
    }
    else
    {
        bool isValid = SimulationPresetEditor::Get().OnImGuiUpdate();

        if (ImGui::Button("Cancel"))
        {
            Entity::SetDebugDraw(false);
            m_currentState = State::Simulating;
        }

        ImGui::SameLine();
        if (!isValid)
        {
            ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
            ImGui::PushStyleVar(
                ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
        }

        SimulationPreset editedPreset =
            SimulationPresetEditor::Get().GetPreset();
        SimulationPresetManager& presetManager = SimulationPresetManager::Get();
        if (ImGui::Button("Save"))
        {
            presetManager.Update(editedPreset);
            presetManager.SavePreset(editedPreset.m_name);
        }

        ImGui::SameLine();
        if (ImGui::Button("Done"))
        {
            Entity::SetDebugDraw(false);
            presetManager.Update(editedPreset);
            m_simulation.LoadPreset(editedPreset);
            m_currentState = State::Simulating;
        }

        if (!isValid)
        {
            ImGui::PopItemFlag();
            ImGui::PopStyleVar();
        }
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
        Entity::SetDebugDraw(true);
        m_currentState = State::EditingPreset;
        SimulationPresetEditor::Get().LoadPreset(
            presetManager.GetPreset(m_simulation.GetCurrentPresetName()));
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

    if (ImGui::Button("Create New Preset"))
    {
        Entity::SetDebugDraw(true);
        m_currentState = State::EditingPreset;
        SimulationPresetEditor::Get().LoadPreset({});
    }
}

void Application::Render()
{
    m_window.clear();

    if (m_currentState == State::Simulating)
    {
        m_simulation.Draw(m_window);
    }
    else
    {
        SimulationPresetEditor::Get().Draw(m_window);
    }

    ImGui::SFML::Update(m_window, m_deltaClock.restart());
    OnImGuiUpdate();
    ImGui::SFML::Render(m_window);
    m_window.display();
}
