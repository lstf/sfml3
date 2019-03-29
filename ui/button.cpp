#ifdef EDITOR_BUILD
#include "button.h"
void Button::draw(sf::RenderTarget& w, sf::RenderStates states) const {
	w.draw(body, states);
	w.draw(text, states);
}

BState Button::handle_input(sf::Event &event, sf::Vector2i m_pos) {
	if (event.type == sf::Event::MouseMoved) {
		if (bounds.contains(m_pos)) {
			setColors(true);
			return BHOVER;
		} else {
			setColors();
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
	setString(text.getString());
}

void Button::setString(string str) {
	text.setString("gl");
	sf::FloatRect text_bounds = text.getGlobalBounds();
	int text_y = bounds.top - (bounds.height - text_bounds.height) / 2;
	text.setString(str);
	text_bounds = text.getGlobalBounds();
	int text_x = bounds.left + (bounds.width - text_bounds.width) / 2;
	text.setPosition((float)text_x, text_y - 2.0);
}

void Button::setColors(bool hover) {
	if (hover) {
		text.setFillColor(hover_t);
		body.setFillColor(hover_b);
		body.setOutlineColor(hover_o);
	} else {
		text.setFillColor(normal_t);
		body.setFillColor(normal_b);
		body.setOutlineColor(normal_o);
	}
}

sf::Vector2f Button::getPosition() {
	return body.getPosition();
}

Button::Button(string t, sf::FloatRect r) {
	normal_b	= UIC_ED_BUTTON_BODY_NORMAL;
	hover_b		= UIC_ED_BUTTON_BODY_HOVER;
	normal_t	= UIC_ED_BUTTON_TEXT_NORMAL;
	hover_t		= UIC_ED_BUTTON_TEXT_HOVER;
	normal_o	= UIC_ED_BUTTON_OUTLINE_NORMAL;
	hover_o		= UIC_ED_BUTTON_OUTLINE_HOVER;

	body = sf::RectangleShape(sf::Vector2f(r.width, r.height));
	body.setPosition(r.left, r.top);
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

	bounds.left		= r.left;
	bounds.top		= r.top;
	bounds.width	= r.width;
	bounds.height	= r.height;

	setColors();
}

#endif
