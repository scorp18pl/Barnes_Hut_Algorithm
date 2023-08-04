#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <Universal/Math/Vector/Vector2f.h>

namespace Utils
{
    Uni::Math::Vector2f CreateUniVectorFromSfVector(const sf::Vector2f& vector);
    sf::Vector2f CreateSfVectorFromUniVector(const Uni::Math::Vector2f& vector);
    void DrawVector(
        sf::RenderWindow& window,
        const Uni::Math::Vector2f& origin,
        const Uni::Math::Vector2f& vector,
        const sf::Color& color = sf::Color::Green);
} // namespace Utils
