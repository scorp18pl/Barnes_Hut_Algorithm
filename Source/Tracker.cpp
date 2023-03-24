#include <Tracker.h>
#include <Utils/Utils.h>

bool Tracker::m_limit = true;

void Tracker::AddVertex(Uni::Math::Vector2f position)
{
    if (m_limit && m_vertices.size() >= 2LU * MAX_LENGTH)
    {
        m_vertices.erase(
            m_vertices.begin(), m_vertices.end() - 2LU * MAX_LENGTH);
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

void Tracker::Draw(sf::RenderWindow* window) const
{
    if (m_vertices.empty())
    {
        return;
    }

    window->draw(m_vertices.data(), m_vertices.size(), sf::Lines);
}

void Tracker::ToggleLimit()
{
    Tracker::m_limit = !Tracker::m_limit;
}
