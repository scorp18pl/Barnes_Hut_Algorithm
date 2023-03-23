#pragma once

#include <Entity/Entity.h>

class Camera : public Entity
{
public:
    Camera();
    Camera(sf::RenderWindow* window);
    ~Camera() = default;

    sf::View GetView();
    Entity* GetFollowed();

    void SetFollowed(Entity* const followedEntity);
    void DisableFollow();
    void Zoom(float zoom);
    void Update() override;

private:
    void HandleKeyInputs();

    static float FRICTION, FORCE, MAX_V;

    bool m_follow;
    Entity* m_followedEntity;
    sf::View m_view;
};
