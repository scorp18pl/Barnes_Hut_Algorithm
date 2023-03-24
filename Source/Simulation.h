#pragma once

#include <Camera.h>
#include <Entity/CircEntity.h>
#include <QuadTree/Node.h>
#include <QuadTree/QuadTree.h>
#include <SFML/Graphics.hpp>
#include <SimulationPreset/SimulationPreset.h>
#include <Universal/Math/BoundingBox2D.h>
#include <algorithm>
#include <memory>
#include <vector>

class Simulation
{
public:
    static Simulation& GetSimulation();

    ~Simulation();

    bool IsRunning() const;
    const SimulationPreset& GetCurrentSimulationPreset() const;

    void Start();

private:
    Simulation();

    static constexpr float DefaultMapSize = 1e8f;

    Camera m_camera;

    SimulationPreset m_currentSimulationPreset;
    static const Uni::Math::BoundingBox2D Map;
    QuadTree* m_quadTree;

    bool m_drawTree;
    size_t m_followedIndex;
    sf::RenderWindow* m_window;
    sf::Clock m_deltaClock;

    static SimulationPreset GenerateDefaultSimulationPreset();

    void PollEvents();
    void Update();
    void Render();
    void OnImGuiUpdate();

    void HandleMouseScroll(float delta);

    void ToggleBarnesHut();
    void ToggleTreeDraw();
    static void ToggleKinematicsDebugDraw();
    static void ToggleTrackerDraw();
    static void ToggleTrackerLimit();

    void FollowPrevious();
    void FollowNext();
    void DisableFollow();
    void ClearTrackers();

    std::vector<Entity*> GetEntityPointers();
};
