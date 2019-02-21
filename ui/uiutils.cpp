#include "uiutils.h"

sf::Vector2i snap(sf::Vector2i v, int s) {
	int x = v.x - v.x % s;
	int y = v.y - v.y % s;
	return sf::Vector2i(x,y);
}

sf::Vector2f snap(sf::Vector2f v, int s) {
	int x = v.x - int(v.x) % s;
	int y = v.y - int(v.y) % s;
	return sf::Vector2f(x,y);
}
