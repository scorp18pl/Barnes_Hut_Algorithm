#include <Entity/CircEntity.h>
#include <Simulation.h>
#include <Universal/Math/Math.h>
#include <Utils/Utils.h>

CircEntity::CircEntity()
    : CircEntity(
          Uni::Math::Vector2f::CreateZero(),
          Uni::Math::Vector2f::CreateZero(),
          0.f)
{
}

CircEntity::CircEntity(
    Uni::Math::Vector2f position,
    Uni::Math::Vector2f velocity,
    float radius,
    float density,
    sf::Color color)
    : Entity(
          position,
          velocity,
          (4.0f / 3.0f) * Uni::Math::Constants::PI * radius * radius * radius *
              density)
{
    m_shape.setPosition(Utils::CreateSfVectorFromUniVector(Entity::m_position));
    m_shape.setRadius(radius);
    m_shape.setFillColor(color);
    m_font = m_font;
}

float CircEntity::GetRadius() const
{
    return m_shape.getRadius();
}

void CircEntity::Draw(sf::RenderWindow* window) const
{
    if (Entity::m_trackerDraw)
    {
        m_tracker.Draw(window);
    }

    window->draw(m_shape);

    if (!Entity::m_debugDraw)
    {
        return;
    }

    const Uni::Math::Vector2f accelerationEndpoint =
        m_position + (m_acceleration);

    sf::Vertex accelerationLine[2] = {
        {
            Utils::CreateSfVectorFromUniVector(m_position),
            sf::Color::Red,
        },
        {
            Utils::CreateSfVectorFromUniVector(accelerationEndpoint),
            sf::Color::Red,
        },
    };

    window->draw(accelerationLine, 2, sf::Lines);

    const Uni::Math::Vector2f velocityEndpoint = m_position + m_velocity;

    sf::Vertex velocityLine[2] = {
        {
            Utils::CreateSfVectorFromUniVector(m_position),
            sf::Color::Green,
        },
        {
            Utils::CreateSfVectorFromUniVector(velocityEndpoint),
            sf::Color::Green,
        },
    };

    window->draw(velocityLine, 2, sf::Lines);
}

void CircEntity::Update()
{
    Entity::Update();
    this->UpdateShapePosition();
}

void CircEntity::UpdateShapePosition()
{
    const Uni::Math::Vector2f shapePosition =
        m_position - Uni::Math::Vector2f::CreateFromFloat(m_shape.getRadius());
    m_shape.setPosition(Utils::CreateSfVectorFromUniVector(shapePosition));
}
