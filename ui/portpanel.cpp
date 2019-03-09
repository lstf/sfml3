#include "portpanel.h"


void Portpanel::draw(sf::RenderTarget& w, sf::RenderStates states) const {
	sf::View temp = w.getView();

	w.setView(w.getDefaultView());
	w.draw(bottom_bg, states);
	w.draw(top_bg, states);
	w.draw(*name_b, states);
	w.draw(*x_b, states);
	w.draw(*y_b, states);
	w.draw(text, states);
	for (int i = 0; i < PORT_B_COUNT; i++) {
		w.draw(*buttons[i].btn, states);
	}
	if (typing) {
		w.draw(*port_tb, states);
	}

	w.setView(temp);
}

void Portpanel::handle_input(
sf::Event &event, sf::Vector2i m_pos, sf::Vector2f w_pos) {
	sf::Vector2f w_pos_snap = snap(w_pos, *sv);

	if (typing) {
		if (pf == PORTAL_NAME) {
			string* sp = port_tb->handle_input(event);
			if (sp) {
				typing = false;
				active_por->target = *sp;
				delete sp;
			}
		} else if (pf == PORTAL_X) {
			string* sp = port_tb->handle_input(event);
			if (sp) {
				typing = false;
				active_por->target_pos.x = stoi(*sp);
				delete sp;
			}
		} else if (pf == PORTAL_Y) {
			string* sp = port_tb->handle_input(event);
			if (sp) {
				typing = false;
				active_por->target_pos.y = stoi(*sp);
				delete sp;
			}
		}
		if (event.type == sf::Event::KeyPressed &&
		event.key.code == sf::Keyboard::Escape) {
			typing = false;
		}
		if (typing == false) {
			delete port_tb;
			port_tb = NULL;
		}
		return;
	}

	BState bs = name_b->handle_input(event, m_pos);
	if (bs) {
		if (bs == BCLICK && selected) {
			pf = PORTAL_NAME;
			string tn = active_por->target;
			typing = true;
			port_tb = new Textbox(
				irfr(name_b->bounds),
				false, tn
			);
			return;
		}
	}
	bs = x_b->handle_input(event, m_pos);
	if (bs) {
		if (bs == BCLICK && selected) {
			sf::Vector2f tp = active_por->target_pos;
			pf = PORTAL_X;
			typing = true;
			port_tb = new Textbox(
				irfr(x_b->bounds),
				true, to_string(int(tp.x))
			);
			return;
		}
	}
	bs = y_b->handle_input(event, m_pos);
	if (bs) {
		if (bs == BCLICK && selected) {
			sf::Vector2f tp = active_por->target_pos;
			pf = PORTAL_Y;
			typing = true;
			port_tb = new Textbox(
				irfr(y_b->bounds),
				true, to_string(int(tp.y))
			);
			return;
		}
	}
	bs = buttons[PORT_B_DOOR].btn->handle_input(event, m_pos);
	if (bs) {
		if (bs == BCLICK && !selected) {
			active_por = (Portal*)new Door;
			selected = true;
			reset(true);
			return;
		}
	}

	if (event.type == sf::Event::MouseButtonPressed) {
		if (event.mouseButton.button == sf::Mouse::Left) {
			if (!selected) {
				for (auto it = Portal::list.begin();
				it != Portal::list.end(); ++it) {
					if ((*it)->bounds().contains(w_pos_snap)) {
						active_por = *it;
						selected = true;
						reset(true);
					}
				}
			} else {
				active_por->set_position(w_pos_snap);
			}
		}
	}
	if (event.type == sf::Event::KeyPressed) {
		if (event.key.code == sf::Keyboard::Delete) {
			if (selected) {
				delete_portal(active_por);
				reset();
			}
		} else if (event.key.code == sf::Keyboard::Escape) {
			reset();
		}
	}
}

void Portpanel::reset(bool retain_sel) {
	if (!retain_sel) {
		selected = false;
	}

	if (typing) {
		typing = false;
		delete port_tb;
	}
	port_tb = NULL;

	if (selected) {
		text.setString(active_por->name);
	} else {
		text.setString("");
	}

	map = game->map_current;
}

Portpanel::Portpanel(Game* _game, SnapVals* _sv) {
	game = _game;
	sv = _sv;

	top_bg.setSize(sf::Vector2f(PORT_TOP_W, PORT_TOP_H));
	top_bg.setPosition(0, PORT_BASE_H);
	top_bg.setFillColor(PORT_BG);
	top_bg.setOutlineColor(PORT_FG);
	top_bg.setOutlineThickness(-1.0);

	bottom_bg.setSize(sf::Vector2f(PORT_BOT_W, PORT_BOT_H));
	bottom_bg.setPosition(0, PORT_BASE_H + PORT_TOP_H);
	bottom_bg.setFillColor(PORT_BG);
	bottom_bg.setOutlineColor(PORT_FG);
	bottom_bg.setOutlineThickness(-1.0);

	text.setFont(*txmap::get_font("./ats/fonts/thintel.ttf"));
	text.setCharacterSize(PORT_B_H);
	text.setFillColor(PORT_FG);
	text.setPosition(PORT_B_W + 14, PORT_BASE_H + 3);

	typing = false;

	reset();

	button_setup();
}

void Portpanel::button_setup() {
	name_b = new Button(
		PORT_BG, PORT_FG, "name", sf::FloatRect(
			(PORT_TOP_W - 2*PORT_B_W) / 2,
			(PORT_TOP_H - 2*PORT_B_H) / 2 + PORT_BASE_H,
			PORT_B_W,
			PORT_B_H
		)
	);
	x_b = new Button(
		PORT_BG, PORT_FG, "x", sf::FloatRect(
			(PORT_TOP_W - 2*PORT_B_W) / 2,
			(PORT_TOP_H - 2*PORT_B_H) / 2 + PORT_BASE_H + PORT_B_H,
			PORT_B_W,
			PORT_B_H
		)
	);
	y_b = new Button(
		PORT_BG, PORT_FG, "y", sf::FloatRect(
			(PORT_TOP_W - 2*PORT_B_W) / 2 + PORT_B_W,
			(PORT_TOP_H - 2*PORT_B_H) / 2 + PORT_BASE_H + PORT_B_H,
			PORT_B_W,
			PORT_B_H
		)
	);

	buttons = new PortButton[PORT_B_COUNT];
	buttons[PORT_B_DOOR].btn = new Button(
		PORT_BG, PORT_FG, "door", sf::FloatRect(
			0,
			PORT_TOP_H + PORT_BASE_H,
			PORT_B_W,
			PORT_B_H
		)
	);
	buttons[PORT_B_DOOR].name = "door";
}
