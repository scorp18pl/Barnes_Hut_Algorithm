#include <Camera.h>

#include "Utils/Utils.h"

float Camera::FRICTION = 0.1f;
float Camera::FORCE = 1e-3f;

Camera::Camera()
    : Entity(
          Uni::Math::Vector2f::CreateZero(),
          Uni::Math::Vector2f::CreateZero(),
          1.0f)
    , m_follow(false)
    , m_view(sf::View())
    , m_followedEntity(nullptr)
{
}

Camera::Camera(sf::RenderWindow* window)
    : Entity(
          Utils::CreateUniVectorFromSfVector(window->getView().getCenter()),
          Uni::Math::Vector2f::CreateZero(),
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

    auto size = Utils::CreateUniVectorFromSfVector(m_view.getSize());

    if (zoom > 0)
    {
        size /= zoom;
    }
    else
    {
        size *= -zoom;
    }

    m_view.setSize(Utils::CreateSfVectorFromUniVector(size));
}

void Camera::Update()
{
    if (m_follow)
    {
        m_position = m_followedEntity->GetPosition();
    }
    else
    {
        HandleKeyInputs();
        Entity::Update();
        m_acceleration = m_velocity * -Camera::FRICTION;
    }

    m_view.setCenter(Utils::CreateSfVectorFromUniVector(m_position));
}

void Camera::HandleKeyInputs()
{
    Uni::Math::Vector2f input = Uni::Math::Vector2f::CreateZero();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        input += Uni::Math::Vector2f{0.0f, -1.0f};
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        input += Uni::Math::Vector2f{0.0f, 1.0f};
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        input += Uni::Math::Vector2f{-1.0f, 0.0f};
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        input += Uni::Math::Vector2f{1.0f, 0.0f};
    }

    float force = Camera::FORCE * m_view.getSize().x;
    ApplyForce(input.GetNormalized() * force);
}
