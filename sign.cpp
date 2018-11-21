#include "sign.h"

sf::FloatRect Sign::bounds() {
	return sp.getGlobalBounds();
}

void Sign::draw(sf::RenderTarget& w, sf::RenderStates states) const {
	w.draw(sp, states);
}

void Sign::interact() {
	dialogBox(file);
}

sf::Vector2f Sign::size() {
	sf::FloatRect r = sp.getGlobalBounds();
	return sf::Vector2f(r.width, r.height);
}

void Sign::place(const sf::Vector2f &p) {
	sp.setPosition(p);
}

Sign::Sign(string f) {
	sp.setTexture(*txmap::get_tx("ats/signs/sign0.png"));
	file = f;
}
