#pragma once

#include "SFML/Graphics.hpp"
#include <iostream>

class Tracker
{
public:
    Tracker() = default;
    ~Tracker() = default;

    void AddVertex(sf::Vector2f position);
    void Clear();
    void Draw(sf::RenderWindow* window);

    static void ToggleLimit();

private:
    static const size_t MAX_LENGTH = 256UL;
    static bool m_limit;
    std::vector<sf::Vertex> m_vertices;
};
