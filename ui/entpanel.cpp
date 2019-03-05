#include "entpanel.h"


void Entpanel::draw(sf::RenderTarget& w, sf::RenderStates states) const {
	sf::View temp = w.getView();

	w.setView(w.getDefaultView());
	w.draw(bottom_bg, states);
	w.draw(top_bg, states);
	w.draw(text, states);
	for (int i = 0; i < ENT_B_COUNT; i++) {
		w.draw(*buttons[i].btn, states);
	}
	if (selected) {
		if (et == ENTT_KEY) {
			w.draw(*kui, states);
		} else if (et == ENTT_KEYLOCK) {
			w.draw(*klui, states);
		}
	}

	w.setView(temp);
	sf::RectangleShape r;
	r.setFillColor(sf::Color::Transparent);
	r.setOutlineColor(sf::Color::Green);
	r.setOutlineThickness(1.0);
	for (auto it = game->ent.list.begin(); it != game->ent.list.end(); ++it) {
		sf::FloatRect eb = (*it)->bounds();
		r.setSize(sf::Vector2f(eb.width, eb.height));
		r.setPosition(eb.left, eb.top);
		w.draw(r, states);
	}
}

void Entpanel::handle_input(sf::Event &event, sf::Vector2i m_pos,
sf::Vector2f w_pos) {
	sf::Vector2f w_pos_snap = snap(w_pos, *sv);
	
	if (selected) {
		if (et == ENTT_KEY) {
			if (kui->handle_input(event, m_pos)) {
				return;
			}
		} else if (et == ENTT_KEYLOCK) {
			if (klui->handle_input(event, m_pos)) {
				return;
			}
		}
	}

	BState bs = buttons[ENT_B_KEY].btn->handle_input(event, m_pos);
	if (bs) {
		if (bs == BCLICK && !selected) {
			active_ent = (Entity*)new KeyItemEnt;
			selected = true;
			reset(true);
			return;
		}
	}
	bs = buttons[ENT_B_KEYLOCK].btn->handle_input(event, m_pos);
	if (bs) {
		if (bs == BCLICK && !selected) {
			active_ent = (Entity*)new KeyLock;
			selected = true;
			reset(true);
			return;
		}
	}

	if (event.type == sf::Event::MouseButtonPressed) {
		if (event.mouseButton.button == sf::Mouse::Left) {
			if (!selected) {
				for (auto it = game->ent.list.begin();
				it != game->ent.list.end(); ++it) {
					if ((*it)->bounds().contains(w_pos_snap)) {
						active_ent = *it;
						selected = true;
						reset(true);
					}
				}
			} else {
				active_ent->set_pos(w_pos_snap);
			}
		}
	}
	if (event.type == sf::Event::KeyPressed) {
		if (event.key.code == sf::Keyboard::Delete) {
			if (selected) {
				delete active_ent;
				reset();
			}
		} else if (event.key.code == sf::Keyboard::Escape) {
			reset();
		}
	}
}

void Entpanel::reset(bool retain_sel) {
	if (!retain_sel) {
		selected = false;
	}

	if (selected) {
		text.setString(active_ent->name);
		sf::FloatRect text_bounds = text.getGlobalBounds();
		text.setPosition(
			(ENT_TOP_W - text_bounds.width) / 2,
			ENT_BASE_H - 4
		);
		if (active_ent->name == "key") {
			et = ENTT_KEY;
			kui->reset((KeyItemEnt*)active_ent);
		} else if (active_ent->name == "keylock") {
			et = ENTT_KEYLOCK;
			klui->reset((KeyLock*)active_ent);
		}
	} else {
		text.setString("");
	}

	map = game->map_current;
}

Entpanel::Entpanel(Game* _game, SnapVals* _sv) {
	game = _game;
	sv = _sv;

	top_bg.setSize(sf::Vector2f(ENT_TOP_W, ENT_TOP_H));
	top_bg.setPosition(0, ENT_BASE_H);
	top_bg.setFillColor(ENT_BG);
	top_bg.setOutlineColor(ENT_FG);
	top_bg.setOutlineThickness(-1.0);

	bottom_bg.setSize(sf::Vector2f(ENT_BOT_W, ENT_BOT_H));
	bottom_bg.setPosition(0, ENT_BASE_H + ENT_TOP_H);
	bottom_bg.setFillColor(ENT_BG);
	bottom_bg.setOutlineColor(ENT_FG);
	bottom_bg.setOutlineThickness(-1.0);

	text.setFont(*txmap::get_font("./ats/fonts/thintel.ttf"));
	text.setCharacterSize(ENT_B_H);
	text.setFillColor(ENT_FG);


	reset();

	button_setup();
	kui = new KeyItemEntUI(8, ENT_BASE_H + 16, ENT_B_W, ENT_B_H);
	klui = new KeyLockUI(8, ENT_BASE_H + 16, ENT_B_W, ENT_B_H);
}

void Entpanel::button_setup() {
	buttons = new EntButton[ENT_B_COUNT];
	buttons[ENT_B_KEY].btn = new Button(
		ENT_BG, ENT_FG, "key", sf::FloatRect(
			0,
			ENT_TOP_H + ENT_BASE_H,
			ENT_B_W,
			ENT_B_H
		)
	);
	buttons[ENT_B_KEY].name = "key";
	buttons[ENT_B_KEYLOCK].btn = new Button(
		ENT_BG, ENT_FG, "key lock", sf::FloatRect(
			ENT_B_W,
			ENT_TOP_H + ENT_BASE_H,
			ENT_B_W,
			ENT_B_H
		)
	);
	buttons[ENT_B_KEYLOCK].name = "key lock";
}
