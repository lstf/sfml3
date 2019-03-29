#ifdef EDITOR_BUILD
#include "portpanel.h"

Portpanel::Portpanel(Game* _game, SnapVals* _sv) {
	log_dbg("constructing portpanel");
	game = _game;
	sv = _sv;

	top_bg.setSize(sf::Vector2f(PORT_TOP_W, PORT_TOP_H));
	top_bg.setPosition(0, PORT_BASE_H);
	top_bg.setFillColor(UIC_ED_BG);

	bottom_bg.setSize(sf::Vector2f(PORT_BOT_W, PORT_BOT_H));
	bottom_bg.setPosition(0, PORT_BASE_H + PORT_TOP_H);
	bottom_bg.setFillColor(UIC_ED_BG);

	text.setFont(*txmap::get_font("./ats/fonts/thintel.ttf"));
	text.setCharacterSize(PORT_B_H);
	text.setFillColor(UIC_ED_BG);
	text.setPosition(PORT_B_W + 14, PORT_BASE_H + 3);

	typing = false;

	reset();

	button_setup();
}

void Portpanel::handle_input(sf::Event &event,
sf::Vector2i m_pos, sf::Vector2f w_pos) {
	sf::Vector2f w_pos_snap = snap(w_pos, *sv);

	if (typing) {
		if (field == PORTAL_NAME) {
			string* sp = port_tb->handle_input(event);
			if (sp) {
				typing = false;
				spawn->target = *sp;
				log_dbg("target set " << *sp);
				delete sp;
			}
		} else if (field == PORTAL_X) {
			string* sp = port_tb->handle_input(event);
			if (sp) {
				typing = false;
				spawn->target_pos.x = stoi(*sp);
				log_dbg("target pos x set " << *sp);
				delete sp;
			}
		} else if (field == PORTAL_Y) {
			string* sp = port_tb->handle_input(event);
			if (sp) {
				typing = false;
				spawn->target_pos.y = stoi(*sp);
				log_dbg("target pos y set " << *sp);
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
			field = PORTAL_NAME;
			typing = true;
			port_tb = new Textbox(
				irfr(name_b->bounds),
				false, spawn->target
			);
			return;
		}
	}
	bs = x_b->handle_input(event, m_pos);
	if (bs) {
		if (bs == BCLICK && selected) {
			field = PORTAL_X;
			typing = true;
			port_tb = new Textbox(
				irfr(x_b->bounds),
				true, to_string((int)spawn->target_pos.x)
			);
			return;
		}
	}
	bs = y_b->handle_input(event, m_pos);
	if (bs) {
		if (bs == BCLICK && selected) {
			field = PORTAL_Y;
			typing = true;
			port_tb = new Textbox(
				irfr(y_b->bounds),
				true, to_string((int)spawn->target_pos.y)
			);
			return;
		}
	}
	bs = buttons[PORT_B_DOOR].btn->handle_input(event, m_pos);
	if (bs) {
		if (bs == BCLICK && !selected) {
			spawn = (PortalSpawner*)new DoorSpawner;
			map->add_port_spawn(spawn);
			selected = true;
			reset(true);
			return;
		}
	}

	if (event.type == sf::Event::MouseButtonPressed) {
		if (event.mouseButton.button == sf::Mouse::Left) {
			if (!selected) {
				for (auto it = map->port_spawn.begin();
				it != map->port_spawn.end(); ++it) {
					if ((*it)->bounds().contains(w_pos_snap)) {
						spawn = *it;
						selected = true;
						reset(true);
					}
				}
			} else {
				spawn->set_pos(w_pos_snap);
			}
		}
	}
	if (event.type == sf::Event::KeyPressed) {
		if (event.key.code == sf::Keyboard::Delete) {
			if (selected) {
				map->remove_port_spawn(spawn);
				delete spawn;
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
		text.setString(spawn->name);
	} else {
		text.setString("");
	}

	map = game->map_current;
}

Portpanel::~Portpanel() {
	log_dbg("destructing portpanel");
}

void Portpanel::button_setup() {
	name_b = new Button(
		"name",
		sf::FloatRect(
			(PORT_TOP_W - 2*PORT_B_W) / 2,
			(PORT_TOP_H - 2*PORT_B_H) / 2 + PORT_BASE_H,
			PORT_B_W,
			PORT_B_H
		)
	);
	x_b = new Button(
		"x",
		sf::FloatRect(
			(PORT_TOP_W - 2*PORT_B_W) / 2,
			(PORT_TOP_H - 2*PORT_B_H) / 2 + PORT_BASE_H + PORT_B_H,
			PORT_B_W,
			PORT_B_H
		)
	);
	y_b = new Button(
		"y",
		sf::FloatRect(
			(PORT_TOP_W - 2*PORT_B_W) / 2 + PORT_B_W,
			(PORT_TOP_H - 2*PORT_B_H) / 2 + PORT_BASE_H + PORT_B_H,
			PORT_B_W,
			PORT_B_H
		)
	);

	buttons = new PortButton[PORT_B_COUNT];
	buttons[PORT_B_DOOR].btn = new Button(
		"door",
		sf::FloatRect(
			0,
			PORT_TOP_H + PORT_BASE_H,
			PORT_B_W,
			PORT_B_H
		)
	);
	buttons[PORT_B_DOOR].name = "door";
}

void Portpanel::draw(sf::RenderTarget& w, sf::RenderStates states) const {
	sf::View temp = w.getView();

	sf::RectangleShape ps_rs;
	ps_rs.setFillColor(sf::Color::Transparent);
	ps_rs.setOutlineColor(sf::Color::Green);
	ps_rs.setOutlineThickness(-1.0);
	for (auto it = map->port_spawn.begin(); it != map->port_spawn.end(); ++it) {
		sf::FloatRect ps_r = (*it)->bounds();
		ps_rs.setPosition(ps_r.left, ps_r.top);
		ps_rs.setSize(sf::Vector2f(ps_r.width, ps_r.height));

		w.draw(*(*it), states);
		w.draw(ps_rs, states);
	}

	w.setView(Window::default_view);
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

#endif
