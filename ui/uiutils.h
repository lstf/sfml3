#ifndef _UIUTILS_O
#define _UIUTILS_O

#include <SFML/Graphics.hpp>

sf::Vector2i snap(sf::Vector2i v, int s);

sf::Vector2f snap(sf::Vector2f v, int s);

sf::Vector2f centerIn(const sf::FloatRect &r1, const sf::FloatRect &r2);
#endif
