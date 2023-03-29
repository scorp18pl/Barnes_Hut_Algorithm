#include <Simulation/Preset/SimulationPresetManager.h>
#include <Simulation/Preset/SimulationPresetSerializer.h>
#include <Simulation/Preset/SimulationPresetUtils.h>

SimulationPresetManager& SimulationPresetManager::Get()
{
    static SimulationPresetManager simulationPresetManager;
    return simulationPresetManager;
}

const std::vector<std::string>& SimulationPresetManager::GetPresetNameList() const
{
    return m_registeredPresetNames;
}

const SimulationPreset& SimulationPresetManager::GetDefaultPreset() const
{
    return GetPreset(SimulationPreset::DefaultName.data());
}

bool SimulationPresetManager::IsNameTaken(
    const std::string& name) const
{
    return m_registeredPresets.contains(name);
}

const SimulationPreset& SimulationPresetManager::GetPreset(
    const std::string& name) const
{
    return m_registeredPresets.at(name);
}

void SimulationPresetManager::Update(
    const SimulationPreset& simulationPreset)
{
    const auto insertionResult = m_registeredPresets.insert({
        simulationPreset.m_name,
        simulationPreset,
    });

    if (insertionResult.second)
    {
        m_registeredPresetNames.push_back(simulationPreset.m_name);
    }
    else
    {
        insertionResult.first->second = simulationPreset;
    }
}

void SimulationPresetManager::RemovePreset(const std::string& name)
{
    if (m_registeredPresets.erase(name))
    {
        m_registeredPresetNames.clear();
        m_registeredPresetNames.reserve(m_registeredPresets.size());
        for (const auto& entry : m_registeredPresets)
        {
            m_registeredPresetNames.push_back(entry.first);
        }
    }
}

void SimulationPresetManager::SavePreset(const std::string& name)
{
    auto simulationPreset = m_registeredPresets.find(name);
    if (simulationPreset == m_registeredPresets.end())
    {
        return;
    }

    SimulationPresetSerializer::WriteSimulationPresetToFile(
        SimulationPresetUtils::GetSimulationPresetFilePath(name),
        simulationPreset->second);
}

SimulationPresetManager::SimulationPresetManager()
{
    LoadSimulationPresets();
}

void SimulationPresetManager::LoadSimulationPresets()
{
    for (const auto& file : std::filesystem::directory_iterator(
             SimulationPresetUtils::GetSimulationPresetDirectoryPath()))
    {
        Update(SimulationPresetSerializer::ReadSimulationPresetFromFile(
            file.path()));
    }
}
