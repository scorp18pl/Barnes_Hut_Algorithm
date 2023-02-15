#pragma once

#include <Universal/Math/Vector2.h>
#include <random>

namespace MyRandom
{
    float CreateRandomFloat(float a = 0.f, float b = 1.f);
    Uni::Math::Vector2f CreateRandomNormalVector2f();
} // namespace MyRandom
