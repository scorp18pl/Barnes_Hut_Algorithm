#pragma once

#include <SFML/Graphics.hpp>
#include <Universal/Math/Vector2.h>
#include <iostream>

class Tracker
{
public:
    Tracker() = default;
    ~Tracker() = default;

    void AddVertex(Uni::Math::Vector2f position);
    void Clear();
    void Draw(sf::RenderWindow* window) const;

    static void ToggleLimit();

private:
    static const size_t MAX_LENGTH = 256UL;
    static bool m_limit;
    std::vector<sf::Vertex> m_vertices;
};
