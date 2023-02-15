#include <Universal/Math/Vector2.h>
#include <SFML/System/Vector2.hpp>

namespace Utils {
    Uni::Math::Vector2f CreateUniVectorFromSfVector(const sf::Vector2f& vector);
    sf::Vector2f CreateSfVectorFromUniVector(const Uni::Math::Vector2f& vector);
}