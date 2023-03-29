#include <Simulation/Preset/SimulationPresetSerializer.h>
#include <Simulation/Preset/SimulationPresetUtils.h>
#include <fstream>
#include <string>

SimulationPreset SimulationPresetSerializer::ReadSimulationPresetFromFile(
    const std::filesystem::path& filePath)
{
    std::ifstream simulationPresetFile(filePath);
    nlohmann::json jsonSimulationPreset =
        nlohmann::json::parse(simulationPresetFile);
    simulationPresetFile.close();

    return DeserializeSimulationPreset(jsonSimulationPreset);
}

void SimulationPresetSerializer::WriteSimulationPresetToFile(
    const std::filesystem::path& filePath, const SimulationPreset& simulationPreset)
{
    nlohmann::json jsonSimulationPreset =
        SerializeSimulationPreset(simulationPreset);
    std::ofstream simulationPresetFile(filePath);

    simulationPresetFile << jsonSimulationPreset;

    simulationPresetFile.close();
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
    jsonSimulationPreset[JsonLabels::Name.data()] = simulationPreset.m_name;
    jsonSimulationPreset[JsonLabels::IsBuiltIn.data()] = simulationPreset.m_isBuiltIn;
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
    simulationPreset.m_name = jsonSimulationPreset[JsonLabels::Name.data()];
    simulationPreset.m_isBuiltIn = jsonSimulationPreset[JsonLabels::IsBuiltIn.data()];

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
