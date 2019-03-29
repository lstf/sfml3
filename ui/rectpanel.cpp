#ifdef EDITOR_BUILD
#include "rectpanel.h"

void Rectpanel::draw(sf::RenderTarget& w, sf::RenderStates states) const {
	sf::View temp = w.getView();

	drawRects(w, states);
	if (right_click) {
		w.draw(add_r, states);
	}

	w.setView(Window::default_view);
	w.draw(bg, states);
	for (int i = 0; i < button_count; i++) {
		w.draw(*buttons[i].btn, states);
	}
	w.draw(*scroll, states);

	w.setView(temp);
}

void Rectpanel::drawRects(sf::RenderTarget& w, sf::RenderStates states) const {
	sf::RectangleShape r(sf::Vector2f(0, 0));
	r.setFillColor(sf::Color::Transparent);
	r.setOutlineColor(sf::Color::Red);
	r.setOutlineThickness(-1.0);
	for (auto it = rects.begin(); it != rects.end(); ++it) {
		r.setPosition(it->left, it->top);
		r.setSize(sf::Vector2f(it->width, it->height));
		w.draw(r, states);
	}
}

void Rectpanel::handle_input(sf::Event &event, sf::Vector2i m_pos,
sf::Vector2f w_pos) {
	sf::Vector2f w_pos_snap = snap(w_pos, *sv);

	//rect buttons
	for (int i = 0; i < button_count; i++) {
		float btn_top = buttons[i].btn->bounds.top;
		if (btn_top + RECT_B_H <= RECT_BASE_H || btn_top >= 480) {
			continue;
		}
		BState bs = buttons[i].btn->handle_input(event, m_pos);
		if (bs == BCLICK) {
			ret_view = new sf::View(sf::FloatRect(0, 0, 960, 480));
			sf::FloatRect r = rects[i];
			ret_view->setCenter(r.left + r.width / 2, r.top + r.height / 2);
			return;
		} else if (bs == BCLICKR) {
			rects.erase(rects.begin() + i);
			reset();
			return;
		}
	}

	//scroll bar
	float scroll_val = scroll->handle_input(event, m_pos);
	for (int i = 0; i < button_count; i++) {
		sf::Vector2f b_pos = buttons[i].btn->getPosition();
		b_pos.y = i * RECT_B_H + RECT_BASE_H + int(scroll_val*scroll_max);
		b_pos.x = 0;
		buttons[i].btn->setPosition(b_pos);
	}

	//world input
	if (event.type == sf::Event::MouseButtonPressed) {
		if (event.mouseButton.button == sf::Mouse::Right) {
			right_click = true;
			add_r.setPosition(w_pos_snap);
			add_r.setSize(sf::Vector2f(0, 0));
		}
	} else if (event.type == sf::Event::MouseButtonReleased) {
		if (event.mouseButton.button == sf::Mouse::Right) {
			if (right_click) {
				sf::Vector2f add_r_pos = add_r.getPosition();
				sf::Vector2f add_r_size = add_r.getSize();
				rects.push_back(orient(sf::FloatRect(
					add_r_pos.x,
					add_r_pos.y,
					add_r_size.x,
					add_r_size.y
				)));
				reset();
			}
		}
	} else if (event.type == sf::Event::MouseMoved) {
		if (right_click) {
			sf::Vector2f add_r_pos = add_r.getPosition();
			add_r.setSize(sf::Vector2f(
				w_pos_snap.x - add_r_pos.x,
				w_pos_snap.y - add_r_pos.y
			));
		}
	}
}

sf::View* Rectpanel::update() {
	sf::View* ret = NULL;
	if (ret_view) {
		ret = ret_view;
		ret_view = NULL;
	}
	return ret;
}

void Rectpanel::reset() {
	right_click = false;
	
	if (buttons) {
		delete buttons;
	}
	button_count = rects.size();
	buttons = new Rectbutton[button_count];
	for (int i = 0; i < button_count; i++) {
		sf::FloatRect r = rects[i];
		ostringstream rect_info;
		rect_info << int(r.left) << "  " << int(r.top) << "  " <<
		int(r.width) << "  " << int(r.height);
		buttons[i].btn = new Button(
			rect_info.str(),
			sf::FloatRect(
				0,
				RECT_BASE_H + i * RECT_B_H,
				RECT_W,
				RECT_B_H
			)
		);
		buttons[i].r = &rects[i];
	}

	if (scroll) {
		delete scroll;
	}
	int scroll_content_height;
	if (button_count == 0) {
		scroll_content_height = RECT_S_W;
	} else {
		sf::IntRect bt = buttons[0].btn->bounds;
		sf::IntRect bb = buttons[button_count - 1].btn->bounds;
		scroll_content_height = bb.top + bb.height - bt.top;
	}
	scroll_max = RECT_S_H - scroll_content_height;
	scroll = new Scrollbar(
		RECT_S_H,
		scroll_content_height,
		RECT_W,
		RECT_BASE_H,
		RECT_S_W,
		RECT_S_H
	);
}

Rectpanel::Rectpanel(SnapVals* _sv) {
	sv = _sv;

	bg = sf::RectangleShape(sf::Vector2f(RECT_W, RECT_S_H));
	bg.setPosition(0, RECT_BASE_H);
	bg.setFillColor(UIC_ED_BG);

	scroll = NULL;
	buttons = NULL;
	reset();

	ret_view = NULL;

	add_r = sf::RectangleShape(sf::Vector2f(0, 0));
	add_r.setFillColor(sf::Color::Transparent);
	add_r.setOutlineColor(sf::Color::Green);
	add_r.setOutlineThickness(-1.0);

	right_click = false;
}

#endif
