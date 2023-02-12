#include "OmniMath.h"

#include <math.h>

namespace OMath
{
    float Q_rsqrt(float number)
    {
        long i;
        float x2, y;
        const float threehalfs = 1.5F;

        x2 = number * 0.5F;
        y = number;
        i = *(long*)&y; // evil floating point bit level hacking
        i = 0x5f3759df - (i >> 1); // what the fuck?
        y = *(float*)&i;
        y = y * (threehalfs - (x2 * y * y)); // 1st iteration
        //	y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration,
        // this can be removed

        return y;
    }

    Vector2f::Vector2f()
    {
        (*this) = Vector2f::CreateZero();
    }

    Vector2f::Vector2f(float x, float y)
        : m_x{ x }
        , m_y{ y }
    {
    }

    Vector2f Vector2f::CreateFromSfVector(sf::Vector2f vector)
    {
        return { vector.x, vector.y };
    }

    Vector2f Vector2f::CreateFromFloat(float value)
    {
        return { value, value };
    }

    Vector2f Vector2f::CreateZero()
    {
        return CreateFromFloat(0.0f);
    }

    float Vector2f::GetLengthSquared() const
    {
        return m_x * m_x + m_y * m_y;
    }

    float Vector2f::GetLength() const
    {
        return sqrt(GetLengthSquared());
    }

    Vector2f Vector2f::GetNormalized() const
    {
        const float lengthInverse = Q_rsqrt(GetLengthSquared());
        return { m_x * lengthInverse, m_y * lengthInverse };
    }

    sf::Vector2f Vector2f::ToSfVector() const
    {
        return { m_x, m_y };
    }

    Vector2f& Vector2f::operator+=(const Vector2f& other)
    {
        m_x += other.m_x;
        m_y += other.m_y;

        return (*this);
    }

    Vector2f Vector2f::operator+(const Vector2f& other) const
    {
        Vector2f vector = (*this);
        vector += other;
        return vector;
    }

    Vector2f& Vector2f::operator-=(const Vector2f& other)
    {
        m_x -= other.m_x;
        m_y -= other.m_y;

        return (*this);
    }

    Vector2f Vector2f::operator-(const Vector2f& other) const
    {
        Vector2f vector = (*this);
        vector -= other;
        return vector;
    }

    Vector2f& Vector2f::operator*=(float value)
    {
        m_x *= value;
        m_y *= value;

        return (*this);
    }

    Vector2f Vector2f::operator*(float value) const
    {
        Vector2f vector = (*this);
        vector *= value;
        return vector;
    }

    Vector2f& Vector2f::operator/=(float value)
    {
        m_x /= value;
        m_y /= value;

        return (*this);
    }

    Vector2f Vector2f::operator/(float value) const
    {
        Vector2f vector = (*this);
        vector /= value;
        return vector;
    }
} // namespace OMath