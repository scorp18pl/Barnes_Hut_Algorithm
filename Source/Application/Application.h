#pragma once

#include <Simulation/Simulation.h>
#include <Application/Camera.h>

class Application
{
public:
    static Application& GetApplication();

    void Start();

    Application(const Application& other) = delete;
    Application(Application&& other) = delete;
private:
    enum class State
    {
        Simulating,
        EditingPreset
    };

    sf::Clock m_deltaClock;
    sf::RenderWindow m_window;

    Camera m_camera;
    Simulation m_simulation;

    State m_currentState{State::Simulating};

    Application();
    ~Application() = default;

    bool IsRunning() const;

    void HandleEvents();
    void HandleMouseScroll(float delta);

    void Update();
    void OnImGuiUpdate();
    void OnImGuiPresetOptionsUpdate();
    void Render();
};
