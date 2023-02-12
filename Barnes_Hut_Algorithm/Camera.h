#pragma once

#include "Entity.h"

enum class Direction
{
    UP,
    RIGHT,
    DOWN,
    LEFT
};

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
    void Move(Direction d);
    void Update() override;

private:
    static float FRICTION, FORCE, MAX_V;

    bool m_follow;
    Entity* m_followedEntity;
    sf::View m_view;
};
