#ifndef _UIUTILS_H
#define _UIUTILS_H

#include <iostream>

#include <SFML/Graphics.hpp>

using namespace std;

struct MenuButton {
	sf::RectangleShape body;
	sf::Text text;
};

#ifdef EDITOR_BUILD
struct SnapVals {
	int x;
	int y;
	int xo;
	int yo;
};

sf::Vector2i snap(sf::Vector2i v, SnapVals sv);

sf::Vector2f snap(sf::Vector2f v, SnapVals sv);

sf::FloatRect orient(sf::FloatRect r);
#endif

sf::Vector2f centerIn(const sf::FloatRect &r1, const sf::FloatRect &r2);

#endif
