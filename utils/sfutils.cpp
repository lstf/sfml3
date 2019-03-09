#include "sfutils.h"

sf::Vector2f vivf(const sf::Vector2i &v) {
	return sf::Vector2f(v.x, v.y);
}

sf::Vector2i vfvi(const sf::Vector2f &v) {
	return sf::Vector2i(v.x, v.y);
}

sf::FloatRect irfr(const sf::IntRect &r) {
	return sf::FloatRect(r.left, r.top, r.width, r.height);
}

sf::IntRect frir(const sf::FloatRect &r) {
	return sf::IntRect(r.left, r.top, r.width, r.height);
}
