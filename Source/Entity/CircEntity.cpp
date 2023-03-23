#include <Entity/CircEntity.h>
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
    float mass,
    float radius,
    sf::Color color)
    : Entity(position, velocity, mass)
{
    m_shape.setPosition(Utils::CreateSfVectorFromUniVector(Entity::m_position));
    m_shape.setRadius(radius);
    m_shape.setFillColor(color);
    m_font = m_font;
}

void CircEntity::Draw(sf::RenderWindow* window)
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
        m_position + (m_acceleration * 10.0f);

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