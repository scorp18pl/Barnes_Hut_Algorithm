#pragma once

#include <Simulation/Entity/CircEntity.h>
#include <Universal/Math/Math.h>
#include <memory>
#include <vector>

struct SimulationPreset
{
    static constexpr std::string_view DefaultName = "Default";

    std::string m_name{ DefaultName.data() };
    bool m_isBuiltIn{ true };
    std::vector<CircEntity> m_entities;
};
