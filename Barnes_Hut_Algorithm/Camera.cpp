#include "Camera.h"

float Camera::FRICTION = 0.1f;
float Camera::FORCE = 1e1f;

Camera::Camera()
    : Entity(OMath::Vector2f::CreateZero(), OMath::Vector2f::CreateZero(), 1.0f)
    , m_follow(false)
    , m_view(sf::View())
    , m_followedEntity(nullptr)
{
}

Camera::Camera(sf::RenderWindow* window)
    : Entity(
          OMath::Vector2f::CreateFromSfVector(window->getView().getCenter()),
          OMath::Vector2f::CreateZero(),
          1.0f)
    , m_follow(false)
    , m_view(window->getView())
    , m_followedEntity(nullptr)
{
}

sf::View Camera::GetView()
{
    return m_view;
}

Entity* Camera::GetFollowed()
{
    return m_followedEntity;
}

void Camera::SetFollowed(Entity* const followedEntity)
{
    m_follow = true;
    m_followedEntity = followedEntity;
}

void Camera::DisableFollow()
{
    m_follow = false;
    m_followedEntity = nullptr;
}

void Camera::Zoom(float zoom)
{
    if (zoom == 0.0f)
    {
        return;
    }

    OMath::Vector2f size =
        OMath::Vector2f::CreateFromSfVector(m_view.getSize());

    if (zoom > 0)
    {
        size /= zoom;
    }
    else
    {
        size *= -zoom;
    }

    m_view.setSize(size.ToSfVector());
}

void Camera::Move(Direction d)
{
    float div = m_view.getSize().x / 1e3f;
    float force = Camera::FORCE * div;

    switch (d)
    {
    case Direction::UP:
        ApplyForce(0.0f, -force);
        break;
    case Direction::RIGHT:
        ApplyForce(force, 0.0f);
        break;
    case Direction::DOWN:
        ApplyForce(0.0f, force);
        break;
    case Direction::LEFT:
        ApplyForce(-force, 0.0f);
        break;
    default:
        break;
    }
}

void Camera::Update()
{
    if (m_follow)
    {
        m_position = m_followedEntity->GetPosition();
    }
    else
    {
        Entity::Update();
        m_acceleration = m_velocity * -Camera::FRICTION;
    }

    m_view.setCenter(m_position.ToSfVector());
}
