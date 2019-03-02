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
			draw_key(w, states);
		}
	}

	w.setView(temp);
}

void Entpanel::draw_key(sf::RenderTarget& w, sf::RenderStates states) const {
	w.draw(*kui.name_b, states);
	w.draw(*kui.levent_b, states);
	if (kui.typing) {
		w.draw(*kui.tb, states);
	}
}

void Entpanel::handle_input(sf::Event &event, sf::Vector2i m_pos,
sf::Vector2f w_pos) {
	sf::Vector2f w_pos_snap = snap(w_pos, *sv);
	
	if (selected) {
		if (et == ENTT_KEY) {
			if (handle_key_input(event, m_pos)) {
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

bool Entpanel::handle_key_input(sf::Event &event, sf::Vector2i m_pos) {
	if (kui.typing) {
		if (kui.field == 0) {
			string* sp = kui.tb->handle_input(event);
			if (sp) {
				kui.typing = false;
				((KeyItemEnt*)active_ent)->key_name = *sp;
				delete sp;
			}
		} else if (kui.field == 1) {
			string* sp = kui.tb->handle_input(event);
			if (sp) {
				kui.typing = false;
				((KeyItemEnt*)active_ent)->levent = *sp;
				delete sp;
			}
		}
		if (event.type == sf::Event::KeyPressed &&
		event.key.code == sf::Keyboard::Escape) {
			kui.typing = false;
		}
		if (kui.typing == false) {
			delete kui.tb;
			kui.tb = NULL;
		}
		return true;
	}

	BState bs = kui.name_b->handle_input(event, m_pos);
	if (bs) {
		if (bs == BCLICK) {
			kui.field = 0;
			string en = ((KeyItemEnt*)active_ent)->key_name;
			kui.typing = true;
			kui.tb = new Textbox(
				irfr(kui.name_b->bounds),
				false, en
			);
			return true;
		}
	} 
	bs = kui.levent_b->handle_input(event, m_pos);
	if (bs) {
		if (bs == BCLICK) {
			kui.field = 1;
			string en = ((KeyItemEnt*)active_ent)->levent;
			kui.typing = true;
			kui.tb = new Textbox(
				irfr(kui.levent_b->bounds),
				false, en
			);
			return true;
		}
	}

	return false;
}

void Entpanel::reset(bool retain_sel) {
	if (!retain_sel) {
		selected = false;
	}

	if (kui.typing) {
		kui.typing = false;
		delete kui.tb;
		kui.tb = NULL;
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

	kui.typing = false;

	reset();

	button_setup();
	key_setup();
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
}

void Entpanel::key_setup() {
	kui.name_b = new Button(
		ENT_BG, ENT_FG, "name", sf::FloatRect(
			8,
			ENT_BASE_H + 16,
			ENT_B_W,
			ENT_B_H
		)
	);
	kui.levent_b = new Button(
		ENT_BG, ENT_FG, "levent", sf::FloatRect(
			8 + ENT_B_W,
			ENT_BASE_H + 16,
			ENT_B_W,
			ENT_B_H
		)
	);
	kui.typing = false;
	kui.tb = NULL;
}
