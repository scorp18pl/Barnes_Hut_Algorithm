#pragma once

#include <SFML/Graphics.hpp>
#include <Tracker.h>
#include <Universal/Math/Vector2.h>
#include <cassert>
#include <iostream>
#include <string>

class Entity
{
public:
    Entity(
        Uni::Math::Vector2f position, Uni::Math::Vector2f velocity, float mass);

    float GetMass();
    Uni::Math::Vector2f GetPosition();

    bool IsDisabled() const;
    Uni::Math::Vector2f CalculateGForce(
        float mass, Uni::Math::Vector2f position) const;

    void Disable();
    void ApplyForce(Uni::Math::Vector2f force);
    void ApplyForce(float f_x, float f_y);
    void ClearAcceleration();

    virtual void Update();

    virtual void Draw(sf::RenderWindow* window);
    static void ToggleDebugDraw();
    static void ToggleTrackerDraw();
    void ClearTracker();

protected:
    sf::Font m_font;
    static bool m_debugDraw;
    static bool m_trackerDraw;

    float m_mass;
    Uni::Math::Vector2f m_position, m_velocity, m_acceleration;
    Tracker m_tracker;
};
