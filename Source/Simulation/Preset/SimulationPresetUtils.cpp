#include <Simulation/Preset/SimulationPresetUtils.h>
#include <fstream>

namespace SimulationPresetUtils
{
    const std::filesystem::path& GetSimulationPresetDirectoryPath()
    {
        static const std::filesystem::path SimulationPresetDirectory =
            std::filesystem::path{ __FILE__ }
                .parent_path()
                .parent_path()
                .parent_path()
                .parent_path()
                .append("data/simulation_presets/");

        return SimulationPresetDirectory;
    }

    std::filesystem::path GetSimulationPresetFilePath(const std::string& name)
    {
        std::filesystem::path simulationPresetDirectory =
            GetSimulationPresetDirectoryPath();

        return simulationPresetDirectory.append(name + ".json");
    }
} // namespace SimulationPresetUtils
