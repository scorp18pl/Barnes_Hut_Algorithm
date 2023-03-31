#include <Simulation/Preset/SimulationPresetGenerator.h>
#include <Simulation/Preset/SimulationPresetManager.h>
#include <Universal/Math/Random.h>
#include <cmath>

static constexpr size_t DefaultEntityCount = 3000LU;
static constexpr float V = 1.0e3f;
static constexpr float DefaultBigRadius = 1.0e5f;

static SimulationPreset GenerateDefaultPreset()
{
    SimulationPreset simulationPreset;

    simulationPreset.m_entities.reserve(DefaultEntityCount);
    simulationPreset.m_entities.emplace_back(
        Uni::Math::Vector2f::CreateZero(),
        Uni::Math::Vector2f::CreateZero(),
        DefaultBigRadius);

    for (size_t i = 1LU; i < DefaultEntityCount; ++i)
    {
        const Uni::Math::Vector2f Position =
            Uni::Math::Vector2f::CreateRandomUnitVector() *
            Uni::Math::Rand::CreateRandomUniformFloat(
                DefaultBigRadius, DefaultBigRadius * 1.0e2f);

        const Uni::Math::Vector2f Velocity =
            Uni::Math::Vector2f::CreateRandomUnitVector() * V;

        const float Radius =
            Uni::Math::Rand::CreateRandomUniformFloat(1.0f, 1.0e3f);

        simulationPreset.m_entities.emplace_back(Position, Velocity, Radius);
    }

    return simulationPreset;
}

static SimulationPreset GenerateTwoBodyPreset()
{
    static constexpr float distanceFromCenter = 1.0e6f;

    SimulationPreset simulationPreset;
    simulationPreset.m_name = "TwoBody";

    CircEntity entity = { Uni::Math::Vector2f::CreateZero(),
                          Uni::Math::Vector2f::CreateZero(),
                          DefaultBigRadius };

    const float speed = std::sqrt(
                            Uni::Math::Constants::BigG * entity.GetMass() /
                            distanceFromCenter) /
        2.0f;

    simulationPreset.m_entities.emplace_back(
        Uni::Math::Vector2f{ distanceFromCenter, 0.0f },
        Uni::Math::Vector2f{ 0.0f, -speed },
        DefaultBigRadius);

    simulationPreset.m_entities.emplace_back(
        Uni::Math::Vector2f{ -distanceFromCenter, 0.0f },
        Uni::Math::Vector2f{ 0.0f, speed },
        DefaultBigRadius);

    return simulationPreset;
}

static SimulationPreset GenerateThreeBodyPreset()
{
    static constexpr float distanceFromCenter = 2.0e6f;

    SimulationPreset simulationPreset;
    simulationPreset.m_name = "ThreeBody";

    CircEntity entity = { Uni::Math::Vector2f::CreateZero(),
                          Uni::Math::Vector2f::CreateZero(),
                          DefaultBigRadius };

    const float speed = std::sqrt(
                            3.0f * Uni::Math::Constants::BigG *
                            entity.GetMass() / distanceFromCenter / 2.0f) /
        2.0f;

    simulationPreset.m_entities.emplace_back(
        Uni::Math::Vector2f{ distanceFromCenter, 0.0f },
        Uni::Math::Vector2f{ 0.0f, -speed },
        DefaultBigRadius);

    simulationPreset.m_entities.emplace_back(
        Uni::Math::Vector2f{ -distanceFromCenter, 0.0f },
        Uni::Math::Vector2f{ 0.0f, speed },
        DefaultBigRadius);

    simulationPreset.m_entities.emplace_back(
        Uni::Math::Vector2f{ 0.0f, 0.0f },
        Uni::Math::Vector2f{ 0.0f, 0.0f },
        DefaultBigRadius);

    return simulationPreset;
}

static SimulationPreset GenerateFiveBodyPreset()
{
    static constexpr float distanceFromCenter = 2.0e6f;

    SimulationPreset simulationPreset;
    simulationPreset.m_name = "FiveBody";

    CircEntity entity = { Uni::Math::Vector2f::CreateZero(),
                          Uni::Math::Vector2f::CreateZero(),
                          DefaultBigRadius };

    const float speed = std::sqrt(
        5.0f * Uni::Math::Constants::BigG * entity.GetMass() /
        distanceFromCenter / 2.0f);

    simulationPreset.m_entities.emplace_back(
        Uni::Math::Vector2f{ distanceFromCenter, 0.0f },
        Uni::Math::Vector2f{ 0.0f, -speed },
        DefaultBigRadius);

    simulationPreset.m_entities.emplace_back(
        Uni::Math::Vector2f{ -distanceFromCenter, 0.0f },
        Uni::Math::Vector2f{ 0.0f, speed },
        DefaultBigRadius);

    simulationPreset.m_entities.emplace_back(
        Uni::Math::Vector2f{ 0.0f, -distanceFromCenter },
        Uni::Math::Vector2f{ -speed, 0.0f },
        DefaultBigRadius);

    simulationPreset.m_entities.emplace_back(
        Uni::Math::Vector2f{ 0.0f, distanceFromCenter },
        Uni::Math::Vector2f{ speed, 0.0f },
        DefaultBigRadius);

    simulationPreset.m_entities.emplace_back(
        Uni::Math::Vector2f{ 0.0f, 0.0f },
        Uni::Math::Vector2f{ 0.0f, 0.0f },
        DefaultBigRadius);

    return simulationPreset;
}

void SimulationPresetGenerator::GenerateBuiltInSimulations()
{
    std::vector<SimulationPreset> builtInSimulations = {
        GenerateDefaultPreset(),
        GenerateTwoBodyPreset(),
        GenerateThreeBodyPreset(),
        GenerateFiveBodyPreset(),
    };

    for (const SimulationPreset& simulationPreset : builtInSimulations)
    {
        SimulationPresetManager::Get().Update(simulationPreset);
        SimulationPresetManager::Get().SavePreset(simulationPreset.m_name);
    }
}
