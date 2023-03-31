#include <Application/Camera.h>
#include <Utils/Utils.h>

Camera& Camera::Get()
{
    static Camera camera;
    return camera;
}

void Camera::SetView(const sf::View& view)
{
    m_view = view;
}

sf::View Camera::GetView()
{
    return m_view;
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
    Entity::Update();
    m_acceleration = m_velocity * -Friction;

    m_view.setCenter(Utils::CreateSfVectorFromUniVector(m_position));
}

Camera::Camera()
    : Entity(
          Uni::Math::Vector2f::CreateZero(),
          Uni::Math::Vector2f::CreateZero(),
          1.0f)
{
}
