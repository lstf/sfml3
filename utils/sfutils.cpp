#include "sfutils.h"

sf::Vector2f vivf(const sf::Vector2i &v) {
	return sf::Vector2f(v.x, v.y);
}

sf::FloatRect irfr(const sf::IntRect &r) {
	return sf::FloatRect(r.left, r.top, r.width, r.height);
}
