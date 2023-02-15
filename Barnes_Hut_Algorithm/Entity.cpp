#include <Entity.h>

#include <Universal/Math/Math.h>
#include <Utils.h>

bool Entity::m_debugDraw = false;
bool Entity::m_trackerDraw = false;

Entity::Entity(
    Uni::Math::Vector2f position, Uni::Math::Vector2f velocity, float mass)
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

Uni::Math::Vector2f Entity::GetPosition()
{
    return m_position;
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

    if (distanceSquared < 1e-4f)
    {
        return { 0.0f, 0.0f };
    }

    const float force =
        Uni::Math::Constants::BigG * mass * m_mass / distanceSquared;

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
    m_position += m_velocity;
    m_velocity += m_acceleration;

    m_tracker.AddVertex(m_position);
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
        m_position + m_acceleration;

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
