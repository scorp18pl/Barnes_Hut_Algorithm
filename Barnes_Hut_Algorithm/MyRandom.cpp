#include <MyRandom.h>
#include <Universal/Math/Math.h>

namespace MyRandom
{
    float CreateRandomFloat(float range_beg, float range_end)
    {
        std::random_device random_device;
        std::mt19937 gen(random_device());
        std::uniform_real_distribution<> distribution(range_beg, range_end);
        return distribution(gen);
    }

    Uni::Math::Vector2f CreateRandomNormalVector2f()
    {
        const float angle = CreateRandomFloat(0, Uni::Math::Constants::TwoPI);
        return {
            static_cast<float>(std::cos(angle)),
            static_cast<float>(std::sin(angle)),
        };
    }
} // namespace MyRandom