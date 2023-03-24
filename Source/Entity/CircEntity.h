#pragma once

#include <Entity/Entity.h>

class CircEntity : public Entity
{
public:
    CircEntity();
    CircEntity(
        Uni::Math::Vector2f position,
        Uni::Math::Vector2f velocity,
        float radius,
        float density = 3.0e6f,
        sf::Color color = sf::Color::White);
    ~CircEntity() = default;

    float GetRadius() const;

    void Draw(sf::RenderWindow* window) const override;
    void Update() override;

private:
    sf::CircleShape m_shape;

    void UpdateShapePosition();
};