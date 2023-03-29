#pragma once

#include <Simulation/Preset/SimulationPreset.h>
#include <Simulation/Preset/SimulationPresetUtils.h>
#include <nlohmann/json.hpp>

class SimulationPresetSerializer
{
public:
    static SimulationPreset ReadSimulationPresetFromFile(
        const std::filesystem::path& filePath);
    static void WriteSimulationPresetToFile(
        const std::filesystem::path& filePath,
        const SimulationPreset& simulationPreset);

private:
    struct JsonLabels
    {
        static constexpr std::string_view Name{ "name" };
        static constexpr std::string_view IsBuiltIn{ "is_built_in" };
        static constexpr std::string_view BigG{ "big_g_value" };
        static constexpr std::string_view Scale{ "scale" };
        static constexpr std::string_view Entities{ "entities" };
        static constexpr std::string_view Position{ "position" };
        static constexpr std::string_view Velocity{ "velocity" };
        static constexpr std::string_view Radius{ "radius" };
    };

    static constexpr std::string_view DefaultFilename{ "default" };

    static std::string SerializeFloat(float value);
    static float DeserializeFloat(const std::string& jsonFloat);

    static nlohmann::json SerializeVector2(const Uni::Math::Vector2f& vector);
    static Uni::Math::Vector2f DeserializeVector2(
        const nlohmann::json& jsonVector2);

    static nlohmann::json SerializeCircEntity(const CircEntity& circEntity);
    static CircEntity DeserializeCircEntity(
        const nlohmann::json& jsonCircEntity);

    static nlohmann::json SerializeSimulationPreset(
        const SimulationPreset& simulationPreset);
    static SimulationPreset DeserializeSimulationPreset(
        const nlohmann::json& jsonSimulationPreset);
};
