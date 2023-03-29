#include "Utils.h"

Uni::Math::Vector2f Utils::CreateUniVectorFromSfVector(
    const sf::Vector2f& vector)
{
    return { vector.x, vector.y };
}

sf::Vector2f Utils::CreateSfVectorFromUniVector(
    const Uni::Math::Vector2f& vector)
{
    return { vector.m_x, vector.m_y };
}

void Utils::DrawVector(
    sf::RenderWindow& window,
    const Uni::Math::Vector2f& origin,
    const Uni::Math::Vector2f& vector,
    const sf::Color& color)
{
    sf::Vertex velocityLine[2] = {
        {
            Utils::CreateSfVectorFromUniVector(origin),
            color,
        },
        {
            Utils::CreateSfVectorFromUniVector(origin + vector),
            color,
        },
    };

    window.draw(velocityLine, 2, sf::Lines);
}
