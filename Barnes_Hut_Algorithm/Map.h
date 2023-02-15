#pragma once

#include <SFML/Graphics.hpp>
#include <Universal/Math/Vector2.h>

class Map
{
public:
    Map(float side = 1e8f);
    Map(Uni::Math::Vector2f starting_pos, Uni::Math::Vector2f ending_pos);

    float GetSide() const;
    Uni::Math::Vector2f GetStartingPosition() const;
    Uni::Math::Vector2f GetEndingPosition() const;

private:
    Uni::Math::Vector2f m_startPosition, m_endPosition;
};
