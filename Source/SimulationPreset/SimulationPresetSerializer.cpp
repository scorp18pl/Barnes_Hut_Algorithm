#include <SimulationPreset/SimulationPresetSerializer.h>
#include <fstream>
#include <string>

SimulationPreset SimulationPresetSerializer::LoadSimulationPreset(
    const std::string& name)
{
    std::ifstream simulationPresetFile(GetSimulationPresetFilePath(name));
    nlohmann::json jsonSimulationPreset =
        nlohmann::json::parse(simulationPresetFile);
    simulationPresetFile.close();

    return DeserializeSimulationPreset(jsonSimulationPreset);
}

void SimulationPresetSerializer::SaveSimulationPreset(
    const SimulationPreset& simulationPreset, const std::string& name)
{
    nlohmann::json jsonSimulationPreset =
        SerializeSimulationPreset(simulationPreset);
    std::ofstream simulationPresetFile(GetSimulationPresetFilePath(name));
    simulationPresetFile << jsonSimulationPreset;
    simulationPresetFile.close();
}

std::filesystem::path SimulationPresetSerializer::
    GetSimulationPresetDirectoryPath()
{
    constexpr std::string_view simulationPresetsRelativePath{
        "data/simulation_presets/"
    };

    return std::filesystem::path{ __FILE__ }
        .parent_path()
        .parent_path()
        .parent_path()
        .append(simulationPresetsRelativePath.data());
}

std::filesystem::path SimulationPresetSerializer::GetSimulationPresetFilePath(
    const std::string& name)
{
    return GetSimulationPresetDirectoryPath().append(name + ".json");
}

std::string SimulationPresetSerializer::SerializeFloat(float value)
{
    return std::to_string(value);
}

float SimulationPresetSerializer::DeserializeFloat(const std::string& jsonFloat)
{
    return std::stof(jsonFloat);
}

nlohmann::json SimulationPresetSerializer::SerializeVector2(
    const Uni::Math::Vector2f& vector)
{
    nlohmann::json vectorJson;
    vectorJson["x"] = SerializeFloat(vector.m_x);
    vectorJson["y"] = SerializeFloat(vector.m_y);

    return vectorJson;
}

Uni::Math::Vector2f SimulationPresetSerializer::DeserializeVector2(
    const nlohmann::json& jsonVector2)
{
    return {
        DeserializeFloat(jsonVector2["x"]),
        DeserializeFloat(jsonVector2["y"]),
    };
}

nlohmann::json SimulationPresetSerializer::SerializeCircEntity(
    const CircEntity& circEntity)
{
    nlohmann::json circEntityJson;
    circEntityJson[JsonLabels::Position.data()] =
        SerializeVector2(circEntity.GetPosition());
    circEntityJson[JsonLabels::Velocity.data()] =
        SerializeVector2(circEntity.GetVelocity());
    circEntityJson[JsonLabels::Radius.data()] =
        SerializeFloat(circEntity.GetRadius());

    return circEntityJson;
}

CircEntity SimulationPresetSerializer::DeserializeCircEntity(
    const nlohmann::json& jsonCircEntity)
{
    return {
        DeserializeVector2(jsonCircEntity[JsonLabels::Position.data()]),
        DeserializeVector2(jsonCircEntity[JsonLabels::Velocity.data()]),
        DeserializeFloat(jsonCircEntity[JsonLabels::Radius.data()]),
    };
}

nlohmann::json SimulationPresetSerializer::SerializeSimulationPreset(
    const SimulationPreset& simulationPreset)
{
    nlohmann::json jsonSimulationPreset;
    jsonSimulationPreset[JsonLabels::BigG.data()] =
        SerializeFloat(simulationPreset.m_bigGValue);
    jsonSimulationPreset[JsonLabels::Scale.data()] =
        SerializeFloat(simulationPreset.m_scale);
    for (const CircEntity& circEntity : simulationPreset.m_entities)
    {
        jsonSimulationPreset[JsonLabels::Entities.data()].push_back(
            SerializeCircEntity(circEntity));
    }

    return jsonSimulationPreset;
}

SimulationPreset SimulationPresetSerializer::DeserializeSimulationPreset(
    const nlohmann::json& jsonSimulationPreset)
{
    SimulationPreset simulationPreset;
    simulationPreset.m_bigGValue =
        DeserializeFloat(jsonSimulationPreset[JsonLabels::BigG.data()]);
    simulationPreset.m_scale =
        DeserializeFloat(jsonSimulationPreset[JsonLabels::Scale.data()]);

    simulationPreset.m_entities.reserve(
        jsonSimulationPreset[JsonLabels::Entities.data()].size());
    for (const nlohmann::json& circEntity :
         jsonSimulationPreset[JsonLabels::Entities.data()])
    {
        auto ptr = DeserializeCircEntity(circEntity);
        simulationPreset.m_entities.push_back(ptr);
    }

    return simulationPreset;
}
