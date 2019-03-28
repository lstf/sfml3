#include "entpanel.h"

void Entpanel::draw(sf::RenderTarget& w, sf::RenderStates states) const {
	sf::View temp = w.getView();

	// Entity spawners
	sf::RectangleShape r;
	r.setFillColor(sf::Color::Transparent);
	r.setOutlineColor(sf::Color::Green);
	r.setOutlineThickness(1.0);
	for (auto it = map->ent_spawn.begin(); it != map->ent_spawn.end(); ++it) {
		//spawner
		w.draw(*(*it), states);

		//green rectangle
		sf::FloatRect eb = (*it)->bounds();
		r.setSize(sf::Vector2f(eb.width, eb.height));
		r.setPosition(eb.left, eb.top);
		w.draw(r, states);
	}

	// UI
	w.setView(Window::default_view);
	w.draw(bottom_bg, states);
	w.draw(top_bg, states);
	w.draw(text, states);
	for (int i = 0; i < ENT_B_COUNT; i++) {
		//entity buttons
		w.draw(*buttons[i].btn, states);
	}
	if (selected) {
		//spawner uis
		if (et == ENTT_ITEM) {
			w.draw(*itui, states);
		} else if (et == ENTT_KEYLOCK) {
			w.draw(*klui, states);
		}
	}

	w.setView(temp);
}

Entpanel::Entpanel(Game* _game, SnapVals* _sv) {
	game = _game;
	sv = _sv;

	top_bg.setSize(sf::Vector2f(ENT_TOP_W, ENT_TOP_H));
	top_bg.setPosition(0, ENT_BASE_H);
	top_bg.setFillColor(UIC_ED_BG);

	bottom_bg.setSize(sf::Vector2f(ENT_BOT_W, ENT_BOT_H));
	bottom_bg.setPosition(0, ENT_BASE_H + ENT_TOP_H);
	bottom_bg.setFillColor(UIC_ED_BG);

	text.setFont(*txmap::get_font("./ats/fonts/thintel.ttf"));
	text.setCharacterSize(ENT_B_H);
	text.setFillColor(UIC_ED_FG);


	reset();

	button_setup();
	itui = new ItemEntUI(8, ENT_BASE_H + 16, ENT_B_W, ENT_B_H);
	klui = new KeyLockUI(8, ENT_BASE_H + 16, ENT_B_W, ENT_B_H);
}

void Entpanel::handle_input(sf::Event &event, sf::Vector2i m_pos,
sf::Vector2f w_pos) {
	sf::Vector2f w_pos_snap = snap(w_pos, *sv);
	
	if (selected) {
		if (et == ENTT_ITEM) {
			if (itui->handle_input(event, m_pos)) {
				return;
			}
		} else if (et == ENTT_KEYLOCK) {
			if (klui->handle_input(event, m_pos)) {
				return;
			}
		}
	}

	BState bs = buttons[ENT_B_ITEM].btn->handle_input(event, m_pos);
	if (bs) {
		if (bs == BCLICK && !selected) {
			spawn = (EntitySpawner*)new ItemEntSpawner;
			map->add_ent_spawn(spawn);
			selected = true;
			reset(true);
			return;
		}
	}
	bs = buttons[ENT_B_KEYLOCK].btn->handle_input(event, m_pos);
	if (bs) {
		if (bs == BCLICK && !selected) {
			spawn = (EntitySpawner*)new KeyLockSpawner;
			map->add_ent_spawn(spawn);
			selected = true;
			reset(true);
			return;
		}
	}

	if (event.type == sf::Event::MouseButtonPressed) {
		if (event.mouseButton.button == sf::Mouse::Left) {
			if (!selected) {
				for (auto it = map->ent_spawn.begin();
				it != map->ent_spawn.end(); ++it) {
					if ((*it)->bounds().contains(w_pos_snap)) {
						spawn = *it;
						selected = true;
						reset(true);
						return;
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
				map->remove_ent_spawn(spawn);
				delete spawn;
				spawn = NULL;
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
		text.setString(spawn->name);
		sf::FloatRect text_bounds = text.getGlobalBounds();
		text.setPosition(
			int((ENT_TOP_W - text_bounds.width) / 2),
			int(ENT_BASE_H - 4)
		);
		if (spawn->name == "item") {
			et = ENTT_ITEM;
			itui->reset((ItemEntSpawner*)spawn);
		} else if (spawn->name == "keylock") {
			et = ENTT_KEYLOCK;
			klui->reset((KeyLockSpawner*)spawn);
		}
	} else {
		text.setString("");
	}

	map = game->map_current;
}

void Entpanel::button_setup() {
	buttons = new EntButton[ENT_B_COUNT];
	buttons[ENT_B_ITEM].btn = new Button(
		"item",
		sf::FloatRect(
			0,
			ENT_TOP_H + ENT_BASE_H,
			ENT_B_W,
			ENT_B_H
		)
	);
	buttons[ENT_B_ITEM].name = "item";
	buttons[ENT_B_KEYLOCK].btn = new Button(
		"key lock",
		sf::FloatRect(
			ENT_B_W,
			ENT_TOP_H + ENT_BASE_H,
			ENT_B_W,
			ENT_B_H
		)
	);
	buttons[ENT_B_KEYLOCK].name = "key lock";
}
