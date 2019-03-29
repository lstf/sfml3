#ifdef EDITOR_BUILD
#include "snappanel.h"

void Snappanel::draw(sf::RenderTarget& w, sf::RenderStates states) const {
	sf::View temp = w.getView();

	w.setView(Window::default_view);
	w.draw(bottom_bg, states);
	for (int i = 0; i < button_count; i++) {
		w.draw(*buttons[i].btn, states);
	}

	w.draw(top_bg, states);
	w.draw(*snap_x_b, states);
	w.draw(*snap_xo_b, states);
	w.draw(*snap_y_b, states);
	w.draw(*snap_yo_b, states);
	w.draw(*add_b, states);
	w.draw(*scroll, states);
	if (typing) {
		w.draw(*snap_tb, states);
	}

	w.setView(temp);
}

void Snappanel::handle_input(sf::Event &event, sf::Vector2i m_pos) {
	if (typing) {
		string* ts = snap_tb->handle_input(event);
		if (ts) {
			if (*ts != "") {
				if (sf == SNAP_VALUE_X) {
					sv->x = stoi(*ts);
					sv->xo %= sv->x;
				} else if (sf == SNAP_OFFSET_X) {
					sv->xo = stoi(*ts);
					sv->xo %= sv->x;
				} else if (sf == SNAP_VALUE_Y) {
					sv->y = stoi(*ts);
					sv->yo %= sv->y;
				} else if (sf == SNAP_OFFSET_Y) {
					sv->yo = stoi(*ts);
					sv->yo %= sv->y;
				}
			}
			delete ts;
			reset();
		}

		if (event.type == sf::Event::KeyPressed &&
		event.key.code == sf::Keyboard::Escape) {
			reset();
		}
		return;
	}
	BState bs = snap_x_b->handle_input(event, m_pos);
	if (bs == BCLICK) {
		typing = true;
		sf = SNAP_VALUE_X;
		snap_tb = new Textbox(
			irfr(snap_x_b->bounds), 
			true,
			to_string(sv->x)
		);
		return;
	}
	bs = snap_xo_b->handle_input(event, m_pos);
	if (bs == BCLICK) {
		typing = true;
		sf = SNAP_OFFSET_X;
		snap_tb = new Textbox(
			irfr(snap_xo_b->bounds), 
			true,
			to_string(sv->xo)
		);
		return;
	}
	bs = snap_y_b->handle_input(event, m_pos);
	if (bs == BCLICK) {
		typing = true;
		sf = SNAP_VALUE_Y;
		snap_tb = new Textbox(
			irfr(snap_y_b->bounds), 
			true,
			to_string(sv->y)
		);
		return;
	}
	bs = snap_yo_b->handle_input(event, m_pos);
	if (bs == BCLICK) {
		typing = true;
		sf = SNAP_OFFSET_Y;
		snap_tb = new Textbox(
			irfr(snap_yo_b->bounds), 
			true,
			to_string(sv->yo)
		);
		return;
	}
	bs = add_b->handle_input(event, m_pos);
	if (bs == BCLICK) {
		svs.push_back(*sv);
		reset();
		return;
	}
	
	for (int i = 0; i < button_count; i++) {
		bs = buttons[i].btn->handle_input(event, m_pos);
		if (bs == BCLICK) {
			*sv = svs[buttons[i].i];
			reset();
		} else if (bs == BCLICKR) {
			svs.erase(svs.begin() + buttons[i].i);
			reset();
		}
	}

	int base_h = SNAP_BASE_H + SNAP_T_H;
	float scroll_val = scroll->handle_input(event, m_pos);
	for (int i = 0; i < button_count; i++) {
		buttons[i].btn->setPosition(sf::Vector2f(
				0,
				base_h + i * SNAP_B_W / 4 + int(scroll_max*scroll_val)
		));
	}

}

void Snappanel::reset() {
	if (snap_x_b) {
		delete snap_x_b;
	}
	snap_x_b = new Button(
		"x: " + to_string(sv->x),
		sf::FloatRect(
			(SNAP_T_W - SNAP_B_W * 2) / 2,
			40 + SNAP_BASE_H,
			SNAP_B_W,
			SNAP_B_W / 4
		)
	);
	if (snap_xo_b) {
		delete snap_xo_b;
	}
	snap_xo_b = new Button(
		"x off: " + to_string(sv->xo),
		sf::FloatRect(
			(SNAP_T_W - SNAP_B_W * 2) / 2 + SNAP_B_W,
			40 + SNAP_BASE_H,
			SNAP_B_W,
			SNAP_B_W / 4
		)
	);
	if (snap_y_b) {
		delete snap_y_b;
	}
	snap_y_b = new Button(
		"y: " + to_string(sv->y),
		sf::FloatRect(
			(SNAP_T_W - SNAP_B_W * 2) / 2,
			56 + SNAP_BASE_H,
			SNAP_B_W,
			SNAP_B_W / 4
		)
	);
	if (snap_yo_b) {
		delete snap_yo_b;
	}
	snap_yo_b = new Button(
		"y off: " + to_string(sv->yo),
		sf::FloatRect(
			(SNAP_T_W - SNAP_B_W * 2) / 2 + SNAP_B_W,
			56 + SNAP_BASE_H,
			SNAP_B_W,
			SNAP_B_W / 4
		)
	);
	if (snap_tb) {
		delete snap_tb;
		snap_tb = NULL;
	}
	typing = false;

	for (int i = 0; i < button_count; i++) {
		delete buttons[i].btn;
	}
	if (buttons) {
		delete [] buttons;
	}

	button_count = svs.size();

	buttons = new SnapButton[button_count];
	int base_h = SNAP_BASE_H + SNAP_T_H;
	for (int i = 0; i < button_count; i++) {
		SnapVals s = svs[i];
		buttons[i].btn = new Button(
			to_string(s.x) + "    " + to_string(s.y) + "    " +
			to_string(s.xo) + "    " + to_string(s.yo),
			sf::FloatRect(
				0,
				base_h + i * SNAP_B_W / 4,
				SNAP_B_W * 2,
				SNAP_B_W / 4
			)
		);
		buttons[i].i = i;
	}

	scroll_setup();

}

Snappanel::Snappanel(SnapVals* _sv) {
	sv = _sv;

	top_bg = sf::RectangleShape(sf::Vector2f(SNAP_T_W, SNAP_T_H));
	top_bg.setPosition(0, SNAP_BASE_H);
	top_bg.setFillColor(UIC_ED_BG);

	bottom_bg = sf::RectangleShape(sf::Vector2f(SNAP_BG_W, SNAP_BG_H));
	bottom_bg.setPosition(0, SNAP_BASE_H + SNAP_T_H);
	bottom_bg.setFillColor(UIC_ED_BG);

	snap_tb		= NULL;
	snap_x_b	= NULL;
	snap_y_b	= NULL;
	snap_xo_b	= NULL;
	snap_yo_b	= NULL;
	add_b = new Button(
		"+",
		sf::FloatRect(
			(SNAP_T_W - SNAP_B_W * 2) / 2,
			72 + SNAP_BASE_H,
			SNAP_B_W * 2,
			SNAP_B_W / 4
		)
	);
	scroll		= NULL;
	buttons		= NULL;
	button_count = 0;
	reset();
}

void Snappanel::scroll_setup() {
	if (scroll) {
		delete scroll;
	}

	int scroll_content_height;

	if (button_count != 0) {
		sf::IntRect bt = buttons[0].btn->bounds;
		sf::IntRect bb = buttons[button_count - 1].btn->bounds;
		scroll_content_height = bb.top + bb.height - bt.top;
	} else {
		scroll_content_height = SNAP_BG_H;
	}

	scroll_max = SNAP_BG_H - scroll_content_height;
	scroll = new Scrollbar(
		SNAP_BG_H,
		scroll_content_height,
		SNAP_BG_W,
		SNAP_BASE_H + SNAP_T_H,
		SNAP_S_W,
		SNAP_S_H
	);
}

#endif
