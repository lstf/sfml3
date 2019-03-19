#include "ilstpanel.h"

void Ilstpanel::draw(sf::RenderTarget& w, sf::RenderStates states) const {
	sf::View temp = w.getView();

	w.setView(Window::default_view);
	w.draw(bg, states);
	w.draw(*plus_b, states);
	for (int i = 0; i < button_count; i++) {
		w.draw(*buttons[i].event_b, states);
		w.draw(*buttons[i].val_b, states);
	}
	w.draw(*scroll, states);

	if (typing) {
		w.draw(*tb, states);
	}

	w.setView(temp);
}

Ilstpanel::Ilstpanel(Game* _game) {
	game = _game;
	map = game->map_current;

	bg.setSize(sf::Vector2f(ILST_W, ILST_S_H));
	bg.setPosition(0, ILST_BASE_H + ILST_PLUS_H);
	bg.setFillColor(ILST_BG);
	bg.setOutlineColor(ILST_FG);
	bg.setOutlineThickness(-1.0);
	
	scroll = NULL;
	scrollview = sf::View(
		sf::Vector2f(ILST_W / 2, ILST_S_H / 2 + ILST_BASE_H),
		sf::Vector2f(ILST_W, ILST_S_H)
	);
	scrollview.setViewport(sf::FloatRect(
		0,
		ILST_BASE_H + ILST_PLUS_H / 480.0,
		ILST_W / 960.0,
		ILST_S_H/480.0
	));
	buttons = NULL;
	tb = NULL;
	reset();

	plus_b = new Button(
		ILST_BG, ILST_FG,
		"+",
		sf::FloatRect(
			0, 
			ILST_BASE_H,
			ILST_PLUS_W,
			ILST_PLUS_H
		)
	);
}

void Ilstpanel::handle_input(sf::Event &event, sf::Vector2i m_pos) {
	if (typing) {
		string* ret = tb->handle_input(event);
		if (ret) {
			if (field == 0) {
				map->init_lstate.erase(event_name);
				map->init_lstate[*ret] = event_val;
			} else if (field == 1) {
				map->init_lstate[event_name] = stoi(*ret);
			}
			reset();
		}
		if (event.type == sf::Event::KeyPressed &&
		event.key.code == sf::Keyboard::Escape) {
			typing = false;
			delete tb;
			tb = NULL;
		}
		return;
	}

	BState bs;
	for (int i = 0; i < button_count; i++) {
		bs = buttons[i].event_b->handle_input(event, m_pos);
		if (bs == BCLICK) {
			typing = true;
			field = 0;
			if (buttons[i].name == "event") {
				tb = new Textbox(irfr(buttons[i].event_b->bounds));
			} else {
				tb = new Textbox(
					irfr(buttons[i].event_b->bounds),
					false, buttons[i].name
				);
			}
			event_name = buttons[i].name;
			event_val = buttons[i].val;
			return;
		} else if (bs == BCLICKR) {
			map->init_lstate.erase(buttons[i].name);
			reset();
		}
		bs = buttons[i].val_b->handle_input(event, m_pos);
		if (bs == BCLICK) {
			typing = true;
			field = 1;
			tb = new Textbox(
				irfr(buttons[i].val_b->bounds), 
				true, to_string(map->init_lstate[buttons[i].name])
			);
			event_name = buttons[i].name;
			event_val = buttons[i].val;
			return;
		} else if (bs == BCLICKR) {
			map->init_lstate.erase(buttons[i].name);
			reset();
		}
	}
	
	bs = plus_b->handle_input(event, m_pos);
	if (bs == BCLICK) {
		map->init_lstate["event"] = 0;
		reset();
		return;
	}
}

void Ilstpanel::reset() {
	cleanup();

	map = game->map_current;

	button_count = map->init_lstate.size();
	if (button_count != 0) {
		buttons = new Ilstbutton[button_count];
	}
	int i = 0;
	for (auto it = map->init_lstate.begin();
	it != map->init_lstate.end(); ++it) {
		buttons[i].name = it->first;
		buttons[i].val = it->second;
		buttons[i].event_b = new Button(
			ILST_BG, ILST_FG,
			it->first,
			sf::FloatRect(
				0,
				ILST_BASE_H + ILST_PLUS_H + i * ILST_B_H,
				ILST_B_E_W,
				ILST_B_H
			)
		);
		buttons[i].val_b = new Button(
			ILST_BG, ILST_FG,
			to_string(it->second),
			sf::FloatRect(
				ILST_B_E_W,
				ILST_BASE_H + ILST_PLUS_H + i * ILST_B_H,
				ILST_B_V_W,
				ILST_B_H
			)
		);
		i++;
	}

	typing = false;

	int scroll_content_height;
	if (button_count == 0) {
		scroll_content_height = ILST_S_W;
	} else {
		sf::IntRect bt = buttons[0].val_b->bounds;
		sf::IntRect bb = buttons[button_count - 1].val_b->bounds;
		scroll_content_height = bb.top + bb.height - bt.top;
	}
	scroll_max = ILST_S_H - scroll_content_height;
	scroll = new Scrollbar(
		ILST_S_H,
		scroll_content_height,
		ILST_W,
		ILST_BASE_H + ILST_PLUS_H,
		ILST_S_W,
		ILST_S_H
	);
}

Ilstpanel::~Ilstpanel() {
	cleanup();
}

void Ilstpanel::cleanup() {
	if (buttons) {
		for (int i = 0; i < button_count; i++) {
			delete buttons[i].event_b;
			delete buttons[i].val_b;
		}
		delete[] buttons;
		buttons = NULL;
	}

	if (scroll) {
		delete scroll;
		scroll = NULL;
	}
	
	if (tb) {
		delete tb;
		tb = NULL;
	}
}
