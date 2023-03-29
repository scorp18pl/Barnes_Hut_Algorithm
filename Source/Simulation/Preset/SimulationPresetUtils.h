#pragma once

#include <filesystem>
#include <vector>

namespace SimulationPresetUtils
{
    const std::filesystem::path& GetSimulationPresetDirectoryPath();
    std::filesystem::path GetSimulationPresetFilePath(const std::string& name);
} // namespace SimulationPresetUtils
