#pragma once

#include <Simulation/Preset/SimulationPreset.h>

class SimulationPresetEditor
{
public:
    static SimulationPresetEditor& Get();

    bool OnImGuiUpdate();
    void Draw(sf::RenderWindow& window);

    void LoadPreset(const SimulationPreset& preset);
    [[nodiscard]] const SimulationPreset& GetPreset() const;

private:
    bool m_isEntitySelected{ false };
    std::string m_selectedEntityLabel;
    size_t m_selectedEntityIndex = 0LU;
    std::array<sf::Vertex, 8> m_selectedLines;

    static std::string CreateEntityLabel(size_t index);
    void CreateNewEntity();
    void SelectEntity(size_t index);
    void ClearSelection();
    void UpdateSelectedShape();

    SimulationPresetEditor() = default;
    SimulationPresetEditor(const SimulationPresetEditor& other) = default;
    SimulationPresetEditor(SimulationPresetEditor&& other) = default;
    ~SimulationPresetEditor() = default;

    SimulationPreset m_preset;
};
