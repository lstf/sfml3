#ifndef _SFUTILS_H
#define _SFUTILS_H

#include <SFML/Graphics.hpp>

sf::Vector2f vivf(const sf::Vector2i &v);

sf::Vector2i vfvi(const sf::Vector2f &v);

sf::FloatRect irfr(const sf::IntRect &r);

sf::IntRect frir(const sf::FloatRect &r);

#endif
