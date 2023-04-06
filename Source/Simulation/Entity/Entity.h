#pragma once

#include <SFML/Graphics.hpp>
#include <Simulation/Entity/Tracker.h>
#include <Universal/Math/Vector2f.h>
#include <cassert>
#include <iostream>
#include <string>

class Entity
{
public:
    Entity(
        Uni::Math::Vector2f position, Uni::Math::Vector2f velocity, float mass);

    bool IsDisabled() const;

    float GetMass() const;
    Uni::Math::Vector2f GetPosition() const;
    Uni::Math::Vector2f GetVelocity() const;

    void SetPosition(const Uni::Math::Vector2f& position);

    void ApplyForce(Uni::Math::Vector2f force);
    void ApplyForce(float f_x, float f_y);
    void ClearAcceleration();

    virtual void Update();

    virtual void Draw(sf::RenderWindow& window) const;
    static void ToggleDebugDraw();
    static void SetDebugDraw(bool value = true);
    void ClearTracker();

protected:
    static bool ShouldDrawVectors;

    sf::Font m_font;
    float m_mass;
    Uni::Math::Vector2f m_position, m_velocity, m_acceleration;
    Tracker m_tracker;
};
