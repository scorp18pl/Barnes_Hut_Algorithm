#pragma once

#include <Simulation/Entity/Entity.h>

class CircEntity : public Entity
{
public:
    static constexpr float MaxEntityRadius = 1.0e5f;
    static constexpr float DefaultDensity = 3.0e6f;
    static constexpr float DefaultRadius = 1.0f;

    CircEntity();
    CircEntity(
        Uni::Math::Vector2f position,
        Uni::Math::Vector2f velocity,
        float radius,
        float density = DefaultDensity,
        sf::Color color = sf::Color::White);
    ~CircEntity() = default;

    float GetRadius() const;
    sf::Color GetColor() const;

    Uni::Math::Vector2f CalculateGForce(
        float mass, Uni::Math::Vector2f position) const;

    Uni::Math::Vector2f CalculateGForce(CircEntity* entity) const;

    void Draw(sf::RenderWindow& window) const override;
    void Update() override;

private:
    sf::CircleShape m_shape;

    void UpdateShapePosition();
};