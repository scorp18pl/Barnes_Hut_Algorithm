#include "Entity.h"

bool Entity::m_debugDraw = false;
bool Entity::m_trackerDraw = false;

Entity::Entity(OMath::Vector2f position, OMath::Vector2f velocity, float mass)
    : m_position(position)
    , m_velocity(velocity)
    , m_mass(mass)
    , m_acceleration(0.0f, 0.0f)
{
}

float Entity::GetMass()
{
    return m_mass;
}

OMath::Vector2f Entity::GetPosition()
{
    return m_position;
}

bool Entity::IsDisabled() const
{
    return m_mass == -1.0f;
}

OMath::Vector2f Entity::CalculateGForce(
    float mass, OMath::Vector2f position) const
{
    const OMath::Vector2f displacementVector = position - m_position;
    const OMath::Vector2f normalVector = displacementVector.GetNormalized();

    const float distanceSquared = displacementVector.GetLengthSquared();

    if (distanceSquared < 1e-4f)
    {
        return { 0.0f, 0.0f };
    }

    const float force =
        OMath::Constants::BigG * mass * m_mass / distanceSquared;

    return normalVector * force;
}

void Entity::Disable()
{
    m_mass = -1.0f;
}

void Entity::ApplyForce(OMath::Vector2f force)
{
    m_acceleration += force / m_mass;
}

void Entity::ApplyForce(float f_x, float f_y)
{
    ApplyForce({ f_x, f_y });
}

void Entity::ClearAcceleration()
{
    m_acceleration = OMath::Vector2f::CreateZero();
}

void Entity::Update()
{
    m_position += m_velocity;
    m_velocity += m_acceleration;

    m_tracker.AddVertex(m_position.ToSfVector());
}

void Entity::Draw(sf::RenderWindow* window)
{
}

void Entity::ToggleDebugDraw()
{
    Entity::m_debugDraw = !Entity::m_debugDraw;
}

void Entity::ToggleTrackerDraw()
{
    Entity::m_trackerDraw = !Entity::m_trackerDraw;
}

void Entity::ClearTracker()
{
    this->m_tracker.Clear();
}

CircEntity::CircEntity()
    : CircEntity(
          OMath::Vector2f::CreateZero(), OMath::Vector2f::CreateZero(), 0.f)
{
}

CircEntity::CircEntity(
    OMath::Vector2f position,
    OMath::Vector2f velocity,
    float mass,
    float radius,
    sf::Color color)
    : Entity(position, velocity, mass)
{
    this->m_shape.setPosition(Entity::m_position.ToSfVector());
    this->m_shape.setRadius(radius);
    this->m_shape.setFillColor(color);
    this->m_font = m_font;
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

    const OMath::Vector2f accelerationEndpoint = m_position + m_acceleration;

    sf::Vertex accelerationLine[2] = {
        {
            m_position.ToSfVector(),
            sf::Color::Red,
        },
        {
            accelerationEndpoint.ToSfVector(),
            sf::Color::Red,
        },
    };

    window->draw(accelerationLine, 2, sf::Lines);

    const OMath::Vector2f velocityEndpoint = m_position + m_velocity;

    sf::Vertex velocityLine[2] = {
        {
            m_position.ToSfVector(),
            sf::Color::Green,
        },
        {
            velocityEndpoint.ToSfVector(),
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
    const OMath::Vector2f shapePosition =
        m_position - OMath::Vector2f::CreateFromFloat(m_shape.getRadius());
    m_shape.setPosition(shapePosition.ToSfVector());
}
