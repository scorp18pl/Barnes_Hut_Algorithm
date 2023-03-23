#pragma once

#include <Entity/Entity.h>

class CircEntity : public Entity
{
public:
    CircEntity();
    CircEntity(
        Uni::Math::Vector2f position,
        Uni::Math::Vector2f velocity,
        float mass,
        float radius = 4.0f,
        sf::Color color = sf::Color::White);
    ~CircEntity() = default;

    void Draw(sf::RenderWindow* window) override;
    void Update() override;

private:
    sf::CircleShape m_shape;

    void UpdateShapePosition();
};