#pragma once

#include <Simulation/Preset/SimulationPreset.h>
#include <filesystem>
#include <string>

class SimulationPresetManager
{
public:
    static SimulationPresetManager& Get();

    const std::vector<std::string>& GetPresetNameList() const;
    const SimulationPreset& GetDefaultPreset() const;
    const SimulationPreset& GetPreset(const std::string& name) const;
    bool IsNameTaken(const std::string& name) const;

    //! Updates registered simulation presets with provided simulation preset.
    //! If a simulation preset with matching name exists, its properties are
    //! updated.
    void Update(const SimulationPreset& simulationPreset);

    //! Removes simulation preset with provided name from registered presets,
    //! and deletes a savefile associated with it if such exists.
    void RemovePreset(const std::string& name);

    void SavePreset(const std::string& name);

private:
    std::unordered_map<std::string, SimulationPreset> m_registeredPresets;
    std::vector<std::string> m_registeredPresetNames;

    SimulationPresetManager();
    SimulationPresetManager(const SimulationPresetManager& other) = default;
    SimulationPresetManager(SimulationPresetManager&& other) = default;
    ~SimulationPresetManager() = default;

    void LoadSimulationPresets();
};
