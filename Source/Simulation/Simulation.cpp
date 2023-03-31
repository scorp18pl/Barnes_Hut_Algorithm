#include <Simulation/Preset/SimulationPresetManager.h>
#include <Simulation/Simulation.h>
#include <Utils/Utils.h>
#include <imgui-SFML.h>
#include <imgui.h>
#include <imgui_stdlib.h>

Simulation::Simulation(const Uni::Math::BoundingBox2D& mapBounds)
    : m_mapBounds{ mapBounds }
    , m_quadTree{ m_mapBounds }
{
}

const std::string& Simulation::GetCurrentPresetName() const
{
    return m_currentPresetName;
}

void Simulation::Update()
{
    for (auto& entity : m_entities)
    {
        entity.Update();
        if (!m_mapBounds.IsPointWithinBounds(entity.GetPosition()))
        {
            Uni::Math::Vector2f newPosition =
                entity.GetPosition() - m_mapBounds.GetMinPoint();
            newPosition = newPosition.GetMod(m_mapBounds.GetDimensions());
            newPosition += m_mapBounds.GetMinPoint();
            entity.SetPosition(newPosition);
        }
    }

    m_quadTree.Update();

    for (auto& entity : m_entities)
    {
        entity.ClearAcceleration();
        m_quadTree.ApplyGForcesToEntity(&entity);
    }
}

void Simulation::OnImGuiUpdate()
{
    if (ImGui::CollapsingHeader("Debug Options"))
    {
        ImGui::Indent(16.0f);
        if (ImGui::Button("Toggle Quadtree Visuals"))
        {
            QuadTree::ToggleDraw();
        }

        if (ImGui::Button("Toggle Vector Visuals"))
        {
            Entity::ToggleDebugDraw();
        }

        if (ImGui::Button("Algorithm"))
        {
            QuadTree::ToggleBarnesHut();
        }
        ImGui::Unindent(16.0f);
    }

    if (ImGui::CollapsingHeader("Tracker Options"))
    {
        ImGui::Indent(16.0f);
        if (ImGui::Button("Toggle Visuals"))
        {
            Tracker::ToggleDraw();
        }

        if (ImGui::Button("Toggle Limit"))
        {
            Tracker::ToggleLimit();
        }

        if (ImGui::Button("Clear"))
        {
            ClearTrackers();
        }
        ImGui::Unindent(16.0f);
    }
}

void Simulation::Draw(sf::RenderWindow& window) const
{
    for (const auto& entity : m_entities)
    {
        entity.Draw(window);
    }

    m_quadTree.Draw(window);
}

void Simulation::LoadPreset(const SimulationPreset& simulationPreset)
{
    m_currentPresetName = simulationPreset.m_name;
    m_entities = simulationPreset.m_entities;
    m_quadTree.Clear();
    m_quadTree.Build(GetEntityPointers());
}

void Simulation::ClearTrackers()
{
    for (auto& entity : m_entities)
    {
        entity.ClearTracker();
    }
}

std::vector<CircEntity*> Simulation::GetEntityPointers()
{
    std::vector<CircEntity*> entityPointers;
    entityPointers.reserve(m_entities.size());

    for (auto& entity : m_entities)
    {
        entityPointers.push_back(&entity);
    }

    return entityPointers;
}
