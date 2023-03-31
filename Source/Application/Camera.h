#pragma once

#include <Simulation/Entity/Entity.h>

class Camera : public Entity
{
public:
    static Camera& Get();

    void SetView(const sf::View& view);
    sf::View GetView();

    void Zoom(float zoom);
    void Update() override;

private:
    Camera();
    Camera(const Camera& other) = default;
    Camera(Camera&& other) = default;
    ~Camera() = default;

    static constexpr float Friction = 0.1f;

    sf::View m_view;
};
