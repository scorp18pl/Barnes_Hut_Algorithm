#include "Map.h"

Map::Map(float side)
    : m_startPosition{ OMath::Vector2f::CreateFromFloat(-side / 2.0f) }
    , m_endPosition{ OMath::Vector2f::CreateFromFloat(side / 2.0f) }
{
}

Map::Map(OMath::Vector2f starting_pos, OMath::Vector2f ending_pos)
    : m_startPosition(starting_pos)
    , m_endPosition(ending_pos)
{
}

float Map::GetSide() const
{
    return m_endPosition.m_x - m_startPosition.m_x;
}

OMath::Vector2f Map::GetStartingPosition() const
{
    return m_startPosition;
}

OMath::Vector2f Map::GetEndingPosition() const
{
    return m_endPosition;
}
