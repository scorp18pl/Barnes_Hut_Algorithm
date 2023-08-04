#include <Simulation/Entity/CircEntity.h>
#include <Universal/Math/Math.h>
#include <Utils/Utils.h>
#include <cmath>

CircEntity::CircEntity()
    : CircEntity(
          Uni::Math::Vector2f::CreateZero(),
          Uni::Math::Vector2f::CreateZero(),
          DefaultRadius)
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
    radius = std::min(MaxEntityRadius, radius);
    radius = std::max(0.0f, radius);

    m_shape.setPosition(Utils::CreateSfVectorFromUniVector(
        position - Uni::Math::Vector2f{ radius }));
    m_shape.setRadius(radius);
    m_shape.setFillColor(color);
    m_font = m_font;
}

float CircEntity::GetRadius() const
{
    return m_shape.getRadius();
}

sf::Color CircEntity::GetColor() const
{
    return m_shape.getFillColor();
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
        m_position - Uni::Math::Vector2f{ m_shape.getRadius() };
    m_shape.setPosition(Utils::CreateSfVectorFromUniVector(shapePosition));
}

Uni::Math::Vector2f CircEntity::CalculateGForce(
    float mass, Uni::Math::Vector2f position) const
{
    const Uni::Math::Vector2f displacementVector = position - m_position;
    const Uni::Math::Vector2f normalVector = displacementVector.GetNormalized();

    const float distanceSquared = displacementVector.GetLengthSquared();

    if (distanceSquared < 1.0f)
    {
        return Uni::Math::Vector2f::CreateZero();
    }

    float force = Uni::Math::Constants::BigG * mass * m_mass / distanceSquared;

    return normalVector * force;
}

Uni::Math::Vector2f CircEntity::CalculateGForce(CircEntity* entity) const
{
    return CalculateGForce(entity->GetMass(), entity->GetPosition());
}
