#include "Entity.h"
#include "Simulation/Simulation.h"
#include "Utils/Utils.h"

bool Entity::ShouldDrawVectors = false;

Entity::Entity(
    Uni::Math::Vector2f position, Uni::Math::Vector2f velocity, float mass)
    : m_position(position)
    , m_velocity(velocity)
    , m_mass(mass)
    , m_acceleration(Uni::Math::Vector2f::CreateZero())
{
}

float Entity::GetMass() const
{
    return m_mass;
}

Uni::Math::Vector2f Entity::GetPosition() const
{
    return m_position;
}

Uni::Math::Vector2f Entity::GetVelocity() const
{
    return m_velocity;
}

bool Entity::IsDisabled() const
{
    return m_mass == -1.0f;
}

Uni::Math::Vector2f Entity::CalculateGForce(
    float mass, Uni::Math::Vector2f position) const
{
    const Uni::Math::Vector2f displacementVector = position - m_position;
    const Uni::Math::Vector2f normalVector = displacementVector.GetNormalized();

    const float distanceSquared = displacementVector.GetLengthSquared();

    if (distanceSquared < 1.0f)
    {
        return { 0.0f, 0.0f };
    }

    const float force =
        Uni::Math::Constants::BigG *
        mass * m_mass / distanceSquared;

    return normalVector * force;
}

void Entity::Disable()
{
    m_mass = -1.0f;
}

void Entity::ApplyForce(Uni::Math::Vector2f force)
{
    m_acceleration += force / m_mass;
}

void Entity::ApplyForce(float f_x, float f_y)
{
    ApplyForce({ f_x, f_y });
}

void Entity::ClearAcceleration()
{
    m_acceleration = Uni::Math::Vector2f::CreateZero();
}

void Entity::Update()
{
    m_velocity += m_acceleration;
    m_position += m_velocity;

    m_tracker.AddVertex(m_position);
}

void Entity::Draw(sf::RenderWindow& window) const
{
}

void Entity::ToggleDebugDraw()
{
    Entity::ShouldDrawVectors = !Entity::ShouldDrawVectors;
}

void Entity::ClearTracker()
{
    m_tracker.Clear();
}
