#include "Utils.h"

namespace Utils
{
    Uni::Math::Vector2f CreateUniVectorFromSfVector(const sf::Vector2f& vector)
    {
        return { vector.x, vector.y };
    }

    sf::Vector2f CreateSfVectorFromUniVector(const Uni::Math::Vector2f& vector)
    {
        return { vector.m_x, vector.m_y };
    }
} // namespace Utils