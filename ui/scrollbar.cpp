#ifdef EDITOR_BUILD
#include "scrollbar.h"

void Scrollbar::draw(sf::RenderTarget& w, sf::RenderStates states) const {
	w.draw(bg, states);
	w.draw(fg, states);
}

float Scrollbar::handle_input(sf::Event &event, sf::Vector2i m_pos) {
	if (max_y == 0) {
		return 0.0;
	}

	//Left click
	if (event.type == sf::Event::MouseButtonPressed &&
	event.mouseButton.button == sf::Mouse::Left) {
		//on scrollbar fg
		if (fg.getGlobalBounds().contains(vivf(m_pos))) {
			clicked = true;
		}
	//Left release
	} else if (event.type == sf::Event::MouseButtonReleased &&
	event.mouseButton.button == sf::Mouse::Left) {
		clicked = false;
	//Mouse move
	} else if (event.type == sf::Event::MouseMoved) {
		if (clicked) {
			float d_y = (m_pos - p_m_pos).y;
			float x = fg.getGlobalBounds().left;

			if (cur_y + d_y > max_y) {
				fg.setPosition(x, bg.getGlobalBounds().top + max_y);
			} else if (cur_y + d_y < 0) {
				fg.setPosition(x, bg.getGlobalBounds().top);
			} else {
				fg.move(0, d_y);
			}

			cur_y = fg.getGlobalBounds().top - bg.getGlobalBounds().top;
		}
	}
	p_m_pos = m_pos;

	return float(cur_y) / float(max_y);
}

Scrollbar::Scrollbar(int h1, int h2, int x, int y, int w, int h) {
	if (h1 >= h2) {
		fg = sf::RectangleShape(sf::Vector2f(w, h));
		fg.setFillColor(UIC_ED_BG);
	} else {
		fg = sf::RectangleShape(sf::Vector2f(w, int(h*float(h1)/h2)));
		fg.setFillColor(UIC_ED_FG);
	}
	fg.setPosition(x, y);
	bg.setSize(sf::Vector2f(w, h));
	bg.setPosition(x, y);
	bg.setFillColor(UIC_ED_BG);

	p_m_pos = sf::Vector2i(0, 0);
	clicked = false;

	sf::FloatRect bg_bounds = bg.getGlobalBounds();
	sf::FloatRect fg_bounds = fg.getGlobalBounds();
	max_y = bg_bounds.top + bg_bounds.height - fg_bounds.top - fg_bounds.height;
	cur_y = 0;
}

#endif
