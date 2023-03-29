#include "Tracker.h"
#include "Utils/Utils.h"

bool Tracker::ShouldLimit = true;
bool Tracker::ShouldDraw = false;

void Tracker::AddVertex(Uni::Math::Vector2f position)
{
    if (ShouldLimit && m_vertices.size() >= 2LU * MaxLength)
    {
        m_vertices.erase(
            m_vertices.begin(), m_vertices.end() - 2LU * MaxLength);
    }

    sf::Vertex vertex;
    vertex.position = Utils::CreateSfVectorFromUniVector(position);
    vertex.color = sf::Color(32, 94, 37, 255);

    if (m_vertices.size() > 1LU)
    {
        m_vertices.push_back(m_vertices.back());
    }
    m_vertices.push_back(vertex);
}

void Tracker::Clear()
{
    m_vertices.clear();
}

void Tracker::Draw(sf::RenderWindow& window) const
{
    if (m_vertices.empty() || !Tracker::ShouldDraw)
    {
        return;
    }

    window.draw(m_vertices.data(), m_vertices.size(), sf::Lines);
}

void Tracker::ToggleLimit()
{
    Tracker::ShouldLimit = !Tracker::ShouldLimit;
}

void Tracker::ToggleDraw()
{
    Tracker::ShouldDraw = !Tracker::ShouldDraw;
}
