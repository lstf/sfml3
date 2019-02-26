#include "button.h"
void Button::draw(sf::RenderTarget& w, sf::RenderStates states) const {
	w.draw(body, states);
	w.draw(text, states);
}

BState Button::handle_input(sf::Event &event, sf::Vector2i m_pos) {
	if (event.type == sf::Event::MouseMoved) {
		if (bounds.contains(m_pos)) {
			text.setFillColor(normal);
			body.setFillColor(hover);
			return BHOVER;
		} else {
			text.setFillColor(hover);
			body.setFillColor(normal);
		}
	} else if (event.type == sf::Event::MouseButtonPressed) {
		if (event.mouseButton.button == sf::Mouse::Left &&
		bounds.contains(m_pos)) {
			return BCLICK;
		} else if (event.mouseButton.button == sf::Mouse::Right &&
		bounds.contains(m_pos)) {
			return BCLICKR;
		}
	}
	return BNONE;
}

void Button::setPosition(sf::Vector2f pos) {
	body.setPosition(pos);
	bounds.left = pos.x;
	bounds.top = pos.y;
	string t = text.getString();
	text.setString("gl");
	sf::FloatRect text_bounds = text.getGlobalBounds();
	int text_y = pos.y - (bounds.height - text_bounds.height) / 2;
	text.setString(t);
	text_bounds = text.getGlobalBounds();
	int text_x = pos.x + (bounds.width - text_bounds.width) / 2;
	text.setPosition((float)text_x, text_y - 2.0);
}

sf::Vector2f Button::getPosition() {
	return body.getPosition();
}

Button::Button(
sf::Color n, sf::Color h, string t, sf::FloatRect r) {
	normal	= n;
	hover	= h;

	body = sf::RectangleShape(sf::Vector2f(r.width, r.height));
	body.setPosition(r.left, r.top);
	body.setFillColor(normal);
	body.setOutlineColor(hover);
	body.setOutlineThickness(-1.0);

	text.setFont(*txmap::get_font("./ats/fonts/thintel.ttf"));
	text.setCharacterSize(r.height);
	text.setString("gl");
	sf::FloatRect text_bounds = text.getGlobalBounds();
	int text_y = r.top - (r.height - text_bounds.height) / 2;
	text.setString(t);
	text_bounds = text.getGlobalBounds();
	int text_x = r.left + (r.width - text_bounds.width) / 2;
	text.setPosition((float)text_x, text_y - 2.0);
	text.setFillColor(hover);

	bounds.left		= r.left;
	bounds.top		= r.top;
	bounds.width	= r.width;
	bounds.height	= r.height;
}
