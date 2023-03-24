#pragma once

#include <Entity/CircEntity.h>
#include <Universal/Math/Math.h>
#include <memory>
#include <vector>

struct SimulationPreset
{
    float m_bigGValue = Uni::Math::Constants::BigG;
    float m_scale = 1.0f;
    std::vector<CircEntity> m_entities;
};
