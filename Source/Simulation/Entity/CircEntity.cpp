#include "CircEntity.h"
#include "Simulation/Simulation.h"
#include "Utils/Utils.h"
#include <Universal/Math/Math.h>

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
    m_shape.setPosition(Utils::CreateSfVectorFromUniVector(
        position - Uni::Math::Vector2f::CreateFromFloat(radius)));
    m_shape.setRadius(radius);
    m_shape.setFillColor(color);
    m_font = m_font;
}

float CircEntity::GetRadius() const
{
    return m_shape.getRadius();
}

void CircEntity::Draw(sf::RenderWindow& window) const
{
    m_tracker.Draw(window);

    window.draw(m_shape);

    if (!Entity::ShouldDrawVectors)
    {
        return;
    }

    Utils::DrawVector(window, m_position, m_acceleration, sf::Color::Red);
    Utils::DrawVector(window, m_position, m_velocity);
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
