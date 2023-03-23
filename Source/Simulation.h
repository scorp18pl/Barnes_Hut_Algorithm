#pragma once

#include <Camera.h>
#include <Entity/CircEntity.h>
#include <QuadTree/Node.h>
#include <QuadTree/QuadTree.h>
#include <SFML/Graphics.hpp>
#include <Universal/Math/BoundingBox2D.h>
#include <algorithm>
#include <memory>
#include <vector>

class Simulation
{
public:
    Simulation(size_t entity_count);
    ~Simulation();

    bool IsRunning() const;

    void Start();

private:
    Uni::Math::BoundingBox2D m_map;
    Camera m_camera;

    const size_t ENTITY_COUNT = 32;
    bool m_drawTree;
    std::vector<std::shared_ptr<CircEntity>> m_entities;

    QuadTree* m_quadTree;

    size_t m_followedIndex;
    sf::RenderWindow* m_window;
    sf::Clock m_deltaClock;

    void GenerateEntities();

    void PollEvents();
    void Update();
    void Render();
    void OnImGuiUpdate();

    void HandleMouseScroll(float delta);

    void ToggleBarnesHut();
    void ToggleTreeDraw();
    void ToggleKinematicsDebugDraw();
    void ToggleTrackerDraw();
    void ToggleTrackerLimit();

    void FollowPrevious();
    void FollowNext();
    void DisableFollow();
    void ClearTrackers();

    std::vector<Entity*> GetEntityPointers();
};
