#include "uiutils.h"

sf::Vector2i snap(sf::Vector2i v, SnapVals sv) {
	if (v.x < 0) {
		v.x -= sv.x;
	}
	v.x -= sv.xo;
	if (v.y < 0) {
		v.y -= sv.y;
	}
	v.y -= sv.yo;
	int x = v.x - v.x % sv.x + sv.xo;
	int y = v.y - v.y % sv.y + sv.yo;
	return sf::Vector2i(x, y);
}

sf::Vector2f snap(sf::Vector2f v, SnapVals sv) {
	if (v.x < 0) {
		v.x -= sv.x;
	}
	v.x -= sv.xo;
	if (v.y < 0) {
		v.y -= sv.y;
	}
	v.y -= sv.yo;
	int x = int(v.x) - int(v.x) % sv.x + sv.xo;
	int y = int(v.y) - int(v.y) % sv.y + sv.yo;
	return sf::Vector2f(x, y);
}

sf::Vector2f centerIn(const sf::FloatRect &r1, const sf::FloatRect &r2) {
	return sf::Vector2f(
		r2.left + (r2.width - r1.width) / 2, 
		r2.top + (r2.height - r1.height) / 2
	);
}

sf::FloatRect orient(sf::FloatRect r) {
	if (r.width < 0) {
		r.left += r.width;
		r.width *= -1;
	} 
	if (r.height < 0) {
		r.top += r.height;
		r.height *= -1;
	}
	return r;
}
