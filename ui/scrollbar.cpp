#include "scrollbar.h"

void Scrollbar::draw(sf::RenderTarget& w, sf::RenderStates states) const {
	w.draw(bg, states);
	w.draw(fg, states);
}

float Scrollbar::handle_input(sf::Event &event, sf::Vector2i m_pos) {
	if (max_y == 0.0) {
		return 0.0;
	}
	//Left click
	if (event.type == sf::Event::MouseButtonPressed &&
	event.mouseButton.button == sf::Mouse::Left) {
		//on scrollbar fg
		sf::Vector2f mouse_pos(m_pos.x, m_pos.y);
		if (fg.getGlobalBounds().contains(mouse_pos)) {
			clicked = true;
		}
	//Left release
	} else if (event.type == sf::Event::MouseButtonReleased &&
	event.mouseButton.button == sf::Mouse::Left) {
		clicked = false;
	//Mouse move
	} else if (event.type == sf::Event::MouseMoved) {
		sf::Vector2f mouse_pos(m_pos.x, m_pos.y);
		if (clicked) {
			float d_y = (mouse_pos - p_mouse_pos).y;
			float x = fg.getGlobalBounds().left;

			if (y + d_y > max_y) {
				fg.setPosition(x, bg.getGlobalBounds().top + max_y);
			} else if (y + d_y < 0) {
				fg.setPosition(x, bg.getGlobalBounds().top);
			} else {
				fg.move(0, d_y);
			}

			y = fg.getGlobalBounds().top - bg.getGlobalBounds().top;
		}
		p_mouse_pos = mouse_pos;
	}
	return y / max_y;
}

Scrollbar::Scrollbar(int h1, int h2, int x, int y, int w, int h) {
	bg = sf::RectangleShape(sf::Vector2f((float)w, (float)h));
	bg.setPosition((float)x,(float)y);
	bg.setFillColor(SCROLL_BG);
	if (h1 >= h2) {
		fg = sf::RectangleShape(sf::Vector2f((float)w, (float)h));
	} else {
		fg = sf::RectangleShape(sf::Vector2f((float)w, h*float(h1)/h2));
	}
	fg.setPosition((float)x,(float)y);
	fg.setFillColor(SCROLL_FG);

	sf::FloatRect bg_bounds = bg.getGlobalBounds();
	sf::FloatRect fg_bounds = fg.getGlobalBounds();
	max_y = bg_bounds.top + bg_bounds.height - fg_bounds.top - fg_bounds.height;
	y = 0;

	clicked = false;
}
