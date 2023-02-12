#pragma once

#include "OmniMath.h"
#include <SFML/Graphics.hpp>

class Map
{
public:
    Map(float side = 1e8f);
    Map(OMath::Vector2f starting_pos, OMath::Vector2f ending_pos);

    float GetSide() const;
    OMath::Vector2f GetStartingPosition() const;
    OMath::Vector2f GetEndingPosition() const;

private:
    OMath::Vector2f m_startPosition, m_endPosition;
};
