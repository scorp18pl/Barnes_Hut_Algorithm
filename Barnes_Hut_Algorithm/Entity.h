#pragma once

#include <cassert>
#include <iostream>
#include <string>

#include "OmniMath.h"
#include "SFML/Graphics.hpp"
#include "Tracker.h"

class Entity
{
public:
    Entity(OMath::Vector2f position, OMath::Vector2f velocity, float mass);

    float GetMass();
    OMath::Vector2f GetPosition();

    bool IsDisabled() const;
    OMath::Vector2f CalculateGForce(float mass, OMath::Vector2f position) const;

    void Disable();
    void ApplyForce(OMath::Vector2f force);
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
    OMath::Vector2f m_position, m_velocity, m_acceleration;
    Tracker m_tracker;
};

class CircEntity : public Entity
{
public:
    CircEntity();
    CircEntity(
        OMath::Vector2f position,
        OMath::Vector2f velocity,
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