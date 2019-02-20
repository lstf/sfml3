#include "textbox.h"

void Textbox::draw(sf::RenderTarget& w, sf::RenderStates states) const {
	w.draw(body, states);
	w.draw(text, states);
}

string* Textbox::handle_input(sf::Event &event) {
	if (event.type == sf::Event::KeyPressed) {
		const int code = event.key.code;
		if (code >=0 && code <= 25) {
			if (sf::Keyboard::isKeyPressed(
			sf::Keyboard::LShift) ||
			sf::Keyboard::isKeyPressed(
			sf::Keyboard::RShift)) {
				left += char(code + 65);
			} else {
				left += char(code + 97);
			}
		} else if (code >= 26 && code <= 35) {
			left += char(code - 26 + 48);
		} else if (code == sf::Keyboard::Backspace) {
			if (left != "") {
				left.pop_back();
			}
		} else if (code == sf::Keyboard::Space) {
			left += ' ';
		} else if (code == sf::Keyboard::Period) {
			left += '.';
		} else if (code == sf::Keyboard::Left) {
			if (left != "") {
				right = left.back() + right;
				left.pop_back();
			}
		} else if (code == sf::Keyboard::Right) {
			if (right != "") {
				left += right.front();
				right.erase(0,1);
			}
		} else if (code == sf::Keyboard::Enter) {
			string* ret = new string;
			*ret = left + right;
			return ret;
		}
		text.setString(left + "|" + right);
	}
	return NULL;
}

Textbox::Textbox(sf::FloatRect r, string t) {
	body = sf::RectangleShape(sf::Vector2f(r.width, r.height));
	body.setPosition(r.left, r.top);
	body.setFillColor(sf::Color::White);

	text.setFont(*txmap::get_font("./ats/fonts/thintel.ttf"));
	text.setCharacterSize(r.height);
	text.setString("gl");
	sf::FloatRect text_bounds = text.getGlobalBounds();
	int text_y = r.top - (r.height - text_bounds.height) / 2;
	left = t;
	text.setString(t);
	text.setPosition(r.left, (float)text_y);
	text.setFillColor(sf::Color::Black);
}
