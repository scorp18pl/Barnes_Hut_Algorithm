#pragma once

#include <Simulation/Entity/CircEntity.h>
#include <Simulation/Preset/SimulationPreset.h>
#include <Simulation/QuadTree/Node.h>
#include <Simulation/QuadTree/QuadTree.h>
#include <Universal/Math/BoundingBox2D.h>
#include <vector>

class Simulation
{
public:
    explicit Simulation(const Uni::Math::BoundingBox2D& mapBounds);

    const std::string& GetCurrentPresetName() const;

    void Update();
    void Draw(sf::RenderWindow& window) const;

    void LoadPreset(const SimulationPreset& simulationPreset);

    void OnImGuiUpdate();

private:
    bool m_shouldDrawTree;

    std::string m_currentPresetName;

    Uni::Math::BoundingBox2D m_mapBounds;
    QuadTree m_quadTree;
    std::vector<CircEntity> m_entities;

    void ClearTrackers();

    std::vector<Entity*> GetEntityPointers();
};
