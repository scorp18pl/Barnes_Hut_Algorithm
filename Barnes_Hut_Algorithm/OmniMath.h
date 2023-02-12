#pragma once

#include <SFML/System/Vector2.hpp>

namespace OMath
{
    namespace Constants {
        constexpr float BigG = 1e-3f;
        constexpr float PI = 3.14159265359f;
        constexpr float HalfPI = 1.57079632679f;
        constexpr float TwoPI = 6.28318530718;
    }

    float Q_rsqrt(float number);

    struct Vector2f
    {
        float m_x, m_y;

        static Vector2f CreateFromSfVector(sf::Vector2f vector);
        static Vector2f CreateFromFloat(float value);
        static Vector2f CreateZero();

        Vector2f();
        Vector2f(const Vector2f& vector) = default;
        Vector2f(Vector2f&& vector) = default;
        ~Vector2f() = default;

        Vector2f(float x, float y);

        float GetLength() const;
        float GetLengthSquared() const;
        Vector2f GetNormalized() const;

        sf::Vector2f ToSfVector() const;

        Vector2f& operator=(const Vector2f& other) = default;
        Vector2f& operator+=(const Vector2f& other);
        Vector2f operator+(const Vector2f& other) const;
        Vector2f& operator-=(const Vector2f& other);
        Vector2f operator-(const Vector2f& other) const;
        Vector2f& operator*=(float value);
        Vector2f operator*(float value) const;
        Vector2f& operator/=(float value);
        Vector2f operator/(float value) const;
    };
} // namespace OMath