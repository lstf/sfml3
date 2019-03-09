#include "glstpanel.h"

void Glstpanel::draw(sf::RenderTarget& w, sf::RenderStates states) const {
	sf::View temp = w.getView();

	w.setView(w.getDefaultView());
	w.draw(bg, states);
	w.draw(*refresh_b, states);
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

Glstpanel::Glstpanel(Game* _game) {
	game = _game;

	bg.setSize(sf::Vector2f(GLST_W, GLST_S_H));
	bg.setPosition(0, GLST_BASE_H + GLST_PLUS_H);
	bg.setFillColor(GLST_BG);
	bg.setOutlineColor(GLST_FG);
	bg.setOutlineThickness(-1.0);
	
	scroll = NULL;
	scrollview = sf::View(
		sf::Vector2f(GLST_W / 2, GLST_S_H / 2 + GLST_BASE_H),
		sf::Vector2f(GLST_W, GLST_S_H)
	);
	scrollview.setViewport(sf::FloatRect(
		0,
		GLST_BASE_H + GLST_PLUS_H / 480.0,
		GLST_W / 960.0,
		GLST_S_H/480.0
	));
	buttons = NULL;
	tb = NULL;
	reset();

	refresh_b = new Button(
		GLST_BG, GLST_FG,
		"refresh",
		sf::FloatRect(
			0, 
			GLST_BASE_H,
			GLST_PLUS_W,
			GLST_PLUS_H
		)
	);
}

void Glstpanel::handle_input(sf::Event &event, sf::Vector2i m_pos) {
	if (typing) {
		string* ret = tb->handle_input(event);
		if (ret) {
			if (field == 0) {
				game->level_state->at(event_name) = stoi(*ret);
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
		bs = buttons[i].val_b->handle_input(event, m_pos);
		if (bs == BCLICK) {
			typing = true;
			field = 0;
			tb = new Textbox(
				irfr(buttons[i].val_b->bounds), 
				true, to_string(game->level_state->at(buttons[i].name))
			);
			event_name = buttons[i].name;
			event_val = buttons[i].val;
			return;
		}	
	}
	
	bs = refresh_b->handle_input(event, m_pos);
	if (bs == BCLICK) {
		reset();
		return;
	}
}

void Glstpanel::reset() {
	cleanup();

	button_count = game->level_state->size();
	if (button_count != 0) {
		buttons = new Glstbutton[button_count];
	}
	int i = 0;
	for (auto it = game->level_state->begin();
	it != game->level_state->end(); ++it) {
		buttons[i].name = it->first;
		buttons[i].val = it->second;
		buttons[i].event_b = new Button(
			GLST_BG, GLST_FG,
			it->first,
			sf::FloatRect(
				0,
				GLST_BASE_H + GLST_PLUS_H + i * GLST_B_H,
				GLST_B_E_W,
				GLST_B_H
			)
		);
		buttons[i].val_b = new Button(
			GLST_BG, GLST_FG,
			to_string(it->second),
			sf::FloatRect(
				GLST_B_E_W,
				GLST_BASE_H + GLST_PLUS_H + i * GLST_B_H,
				GLST_B_V_W,
				GLST_B_H
			)
		);
		i++;
	}

	typing = false;

	int scroll_content_height;
	if (button_count == 0) {
		scroll_content_height = GLST_S_W;
	} else {
		sf::IntRect bt = buttons[0].val_b->bounds;
		sf::IntRect bb = buttons[button_count - 1].val_b->bounds;
		scroll_content_height = bb.top + bb.height - bt.top;
	}
	scroll_max = GLST_S_H - scroll_content_height;
	scroll = new Scrollbar(
		GLST_S_H,
		scroll_content_height,
		GLST_W,
		GLST_BASE_H + GLST_PLUS_H,
		GLST_S_W,
		GLST_S_H
	);
}

Glstpanel::~Glstpanel() {
	cleanup();
}

void Glstpanel::cleanup() {
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
