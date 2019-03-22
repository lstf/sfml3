#include "snappanel.h"

void Snappanel::draw(sf::RenderTarget& w, sf::RenderStates states) const {
	sf::View temp = w.getView();

	w.setView(Window::default_view);
	w.draw(bg, states);
	w.draw(*snap_x_b, states);
	w.draw(*snap_xo_b, states);
	w.draw(*snap_y_b, states);
	w.draw(*snap_yo_b, states);
	if (typing) {
		w.draw(*snap_tb, states);
	}

	w.setView(temp);
}

void Snappanel::handle_input(sf::Event &event, sf::Vector2i m_pos) {
	if (typing) {
		string* ts = snap_tb->handle_input(event);
		if (ts) {
			if (*ts == "") {
				reset();
				return;
			}
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
}

void Snappanel::reset() {
	if (snap_x_b) {
		delete snap_x_b;
	}
	snap_x_b = new Button(
		"x: " + to_string(sv->x),
		sf::FloatRect(
			(SNAP_WIDTH - SNAP_B_WIDTH * 2) / 2,
			24,
			SNAP_B_WIDTH,
			SNAP_B_WIDTH / 4
		)
	);
	if (snap_xo_b) {
		delete snap_xo_b;
	}
	snap_xo_b = new Button(
		"x off: " + to_string(sv->xo),
		sf::FloatRect(
			(SNAP_WIDTH - SNAP_B_WIDTH * 2) / 2 + SNAP_B_WIDTH,
			24,
			SNAP_B_WIDTH,
			SNAP_B_WIDTH / 4
		)
	);
	if (snap_y_b) {
		delete snap_y_b;
	}
	snap_y_b = new Button(
		"y: " + to_string(sv->y),
		sf::FloatRect(
			(SNAP_WIDTH - SNAP_B_WIDTH * 2) / 2,
			40,
			SNAP_B_WIDTH,
			SNAP_B_WIDTH / 4
		)
	);
	if (snap_yo_b) {
		delete snap_yo_b;
	}
	snap_yo_b = new Button(
		"y off: " + to_string(sv->yo),
		sf::FloatRect(
			(SNAP_WIDTH - SNAP_B_WIDTH * 2) / 2 + SNAP_B_WIDTH,
			40,
			SNAP_B_WIDTH,
			SNAP_B_WIDTH / 4
		)
	);
	if (snap_tb) {
		delete snap_tb;
		snap_tb = NULL;
	}
	typing = false;
}

Snappanel::Snappanel(SnapVals* _sv) {
	sv = _sv;

	bg = sf::RectangleShape(sf::Vector2f(SNAP_WIDTH, SNAP_HEIGHT));
	bg.setPosition(0, SNAP_BASE_H);
	bg.setFillColor(UIC_ED_BG);

	snap_tb		= NULL;
	snap_x_b	= NULL;
	snap_y_b	= NULL;
	snap_xo_b	= NULL;
	snap_yo_b	= NULL;
	reset();
}
