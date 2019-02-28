#include "mappanel.h"

void Mappanel::draw(sf::RenderTarget& w, sf::RenderStates states) const {
	sf::View temp = w.getView();

	w.setView(w.getDefaultView());
	w.draw(top_bg, states);
	w.draw(bottom_bg, states);
	w.draw(selected_text, states);
	for (unsigned int i = 1; i < 7; i++) {
		w.draw(*top_buttons[i], states);
	}
	if (confirm) {
		w.draw(*top_buttons[0], states);
	}
	if (naming) {
		w.draw(*name_tb, states);
	}

	w.setView(scrollview);
	for (unsigned int i = 0; i < button_count; i++) {
		w.draw(*(buttons[i].btn), states);
	}

	w.setView(w.getDefaultView());
	w.draw(*scroll, states);

	w.setView(temp);
}

MappanelTrans* Mappanel::update() {
	if (naming) {
		name_tb->update();
	}

	MappanelTrans* ret = new MappanelTrans;
	if (load_level) {
		load_level = false;
		if (selected < 0) {
			ret->name = game->map_current->name;
		} else {
			ret->name = game->map_names[selected];
		}
		ret->is_new = false;
		if (ret && ret->name == "null map") {
			delete ret;
			ret = NULL;
		}
	} else if (new_level) {
		new_level = false;
		ret->name = new_level_name;
		ret->is_new = true;
	} else if (delete_level) {
		delete_level = false;
		string s_name;
		if (selected < 0) {
			ret->name = "null map";
			s_name = game->map_current->name;
		} else {
			ret->name = "";
			s_name = game->map_names[selected];
		}
		ret->is_new = false;
		for (int i = game->map_names.size()-1; i >= 0; i++) {
			if (game->map_names[i] == s_name) {
				auto bn = game->map_names.begin();
				game->map_names.erase(bn + i);
				break;
			}
		}
	} else {
		delete ret;
		ret = NULL;
	}

	return ret;
}

void Mappanel::handle_input(sf::Event &event, sf::Vector2i m_pos) {
	//top buttons
	if (naming) {
		string* ts = name_tb->handle_input(event);
		if (ts) {
			vector<string>* m_n = &game->map_names;
			for (auto it = m_n->begin(); it != m_n->end(); ++it) {
				if (*ts == *it) {
					return;
					break;
				}
			}
			switch (nstate) {
			case MAP_NAME_NEW:
				new_level = true;
				new_level_name = *ts;
				break;
			case MAP_NAME_DUP:
				//TODO implement duplicate
				break;
			case MAP_NAME_REN:
				//TODO implement rename
				break;
			}
			naming = false;
			delete name_tb;
		}
		if (event.type == sf::Event::KeyPressed &&
		event.key.code == sf::Keyboard::Escape) {
			naming = false;
			delete name_tb;
		}
		return;
	} 
	if (confirm) {
		BState bs = top_buttons[0]->handle_input(event, m_pos);
		if (bs == BCLICK) {
			confirm = false;
			delete_level = true;
		}
		if (event.type == sf::Event::KeyPressed &&
		event.key.code == sf::Keyboard::Escape) {
			confirm = false;
		}
		return;
	}
	for (int i = 1; i < 7; i++) {
		BState bs = top_buttons[i]->handle_input(event, m_pos);
		if (bs == BCLICK) {
			string s_name;
			switch (i) {
			case 1:
				nstate = MAP_NAME_NEW;
				name_tb = new Textbox(irfr(top_buttons[1]->bounds));
				naming = true;
				break;
			case 2:
				confirm = true;
				break;
			case 3:
				nstate = MAP_NAME_DUP;
				if (selected < 0) {
					s_name = game->map_current->name;
				} else {
					s_name = game->map_names[selected];
				}
				name_tb = new Textbox(
					irfr(top_buttons[3]->bounds),
					false,
					s_name
				);
				naming = true;
				break;
			case 4:
				nstate = MAP_NAME_REN;
				if (selected < 0) {
					s_name = game->map_current->name;
				} else {
					s_name = game->map_names[selected];
				}
				name_tb = new Textbox(
					irfr(top_buttons[4]->bounds),
					false,
					s_name
				);
				naming = true;
				break;
			case 5:
				if (game->map_current->name == "null map") {
					break;
				}
				game->map_current->saveMap();
				selected = -1;
				selected_text.setString(game->map_current->name + " [s]");
				selected_text.setPosition(
					(MAP_T_WIDTH - 
					selected_text.getGlobalBounds().width) / 2,
					(MAP_T_HEIGHT - 
					selected_text.getGlobalBounds().height) / 2 - 48
				);
				break;
			case 6:
				load_level = true;
				break;
			};
			return;
		}
	}

	//map buttons
	for (unsigned int i = 0; i < button_count; i++) {
		BState bs = buttons[i].btn->handle_input(event, m_pos);	
		//hover to show preview
		if (bs == BCLICK) {
			selected = i;
			selected_text.setString("[" +game->map_names[i] + "]");
			selected_text.setPosition(
				(MAP_T_WIDTH - selected_text.getGlobalBounds().width) / 2,
				(MAP_T_HEIGHT - selected_text.getGlobalBounds().height) / 2 - 48
			);
		}
	}

	if (event.type == sf::Event::KeyPressed &&
	event.key.code == sf::Keyboard::Escape) {
		selected = -1;
		selected_text.setString(game->map_current->name);
		selected_text.setPosition(
			(MAP_T_WIDTH - selected_text.getGlobalBounds().width) / 2,
			(MAP_T_HEIGHT - selected_text.getGlobalBounds().height) / 2 - 48
		);
	}
}

void Mappanel::reset() {
	selected = -1;
	selected_text.setString(game->map_current->name);
	selected_text.setPosition(
		(MAP_T_WIDTH - selected_text.getGlobalBounds().width) / 2,
		(MAP_T_HEIGHT - selected_text.getGlobalBounds().height) / 2 - 48
	);
	confirm = false;
	if (naming && name_tb) {
		delete name_tb;
		name_tb = NULL;
	}
	naming = false;
	load_level = false;
	new_level = false;
	delete_level = false;

	for (unsigned int i = 0; i < button_count; i++) {
		delete buttons[i].btn;
	}

	button_count = game->map_names.size();
	buttons = new Mapbutton[button_count];
	for (unsigned int i = 0; i < button_count; i++) {
		buttons[i].btn = new Button(
			MAP_BG, MAP_FG,
			game->map_names[i],
			sf::FloatRect(
				(i%2)*MAP_B_WIDTH,
				(i/2)*MAP_B_WIDTH/4+MAP_BASE_H+MAP_T_HEIGHT,
				MAP_B_WIDTH,
				MAP_B_WIDTH/4
			)
		);
		buttons[i].name = game->map_names[i];
	}
}

Mappanel::Mappanel(Game* _game) {
	game = _game;
	bg_setup();
	button_setup();
	scroll_setup();
}

void Mappanel::bg_setup() {
	top_bg =
	sf::RectangleShape(sf::Vector2f(MAP_T_WIDTH, MAP_T_HEIGHT));
	top_bg.setPosition(0, MAP_BASE_H);
	top_bg.setFillColor(MAP_BG);
	top_bg.setOutlineColor(MAP_FG);
	top_bg.setOutlineThickness(-1.0);

	bottom_bg =
	sf::RectangleShape(sf::Vector2f(MAP_F_WIDTH, 480-MAP_T_HEIGHT-MAP_BASE_H));
	bottom_bg.setPosition(0, MAP_BASE_H + MAP_T_HEIGHT);
	bottom_bg.setFillColor(MAP_BG);
	bottom_bg.setOutlineColor(MAP_FG);
	bottom_bg.setOutlineThickness(-1.0);
}

void gen_top_button(Button** btn, int i, const string &s) {
	if (i > 0) {
		i--;
		*btn = new Button(
			MAP_BG, MAP_FG,
			s,
			sf::FloatRect(
				(i%2)*MAP_B_WIDTH+8,
				(i/2)*MAP_B_WIDTH/4+MAP_BASE_H+40,
				MAP_B_WIDTH,
				MAP_B_WIDTH/4
			)
		);
		return;
	} 
	*btn = new Button(
		MAP_BG, MAP_FG,
		s,
		sf::FloatRect(
			MAP_B_WIDTH+8,
			MAP_BASE_H+40,
			MAP_B_WIDTH,
			MAP_B_WIDTH/4
		)
	);
}

void Mappanel::button_setup() {
	string names[7] = {
		"confirm",
		"new",
		"delete",
		"duplicate",
		"rename",
		"save",
		"load"
	};
	for (int i = 0; i < 7; i++) {
		gen_top_button(&top_buttons[i], i, names[i]);
	}
	confirm = false;
	naming = false;
	load_level = false;
	new_level = false;
	delete_level = false;
	selected_text.setFont(*txmap::get_font("./ats/fonts/thintel.ttf"));
	selected_text.setCharacterSize(32);
	selected_text.setString(game->map_current->name);
	selected_text.setPosition(
		(MAP_T_WIDTH - selected_text.getGlobalBounds().width) / 2,
		(MAP_T_HEIGHT - selected_text.getGlobalBounds().height) / 2 - 48
	);
	selected_text.setFillColor(sf::Color::Black);
	selected = -1;

	button_count = game->map_names.size();
	buttons = new Mapbutton[button_count];
	for (unsigned int i = 0; i < button_count; i++) {
		buttons[i].btn = new Button(
			MAP_BG, MAP_FG,
			game->map_names[i],
			sf::FloatRect(
				(i%2)*MAP_B_WIDTH,
				(i/2)*MAP_B_WIDTH/4+MAP_BASE_H+MAP_T_HEIGHT,
				MAP_B_WIDTH,
				MAP_B_WIDTH/4
			)
		);
		buttons[i].name = game->map_names[i];
	}
}

void Mappanel::scroll_setup() {
	int scroll_content_height;
	if (button_count != 0) {
		sf::IntRect bt = buttons[0].btn->bounds;
		sf::IntRect bb = buttons[button_count - 1].btn->bounds;
		scroll_content_height = bb.top + bb.height - bt.top;
	} else {
		scroll_content_height = 480 - MAP_T_HEIGHT - MAP_BASE_H;
	}
	
	scrollview = sf::View(
		sf::Vector2f(
		MAP_F_WIDTH / 2, (480 - MAP_T_HEIGHT - MAP_BASE_H) / 2 +
		MAP_T_HEIGHT + MAP_BASE_H),
		sf::Vector2f(MAP_F_WIDTH, 480 - MAP_T_HEIGHT - MAP_BASE_H)
	);
	scrollview.setViewport(sf::FloatRect(
		0,
		(MAP_T_HEIGHT+MAP_BASE_H)/480.0,
		MAP_F_WIDTH/960.0,
		(480 - MAP_T_HEIGHT - MAP_BASE_H)/480.0
	));
	scroll_max = 480 - MAP_T_HEIGHT - MAP_BASE_H - scroll_content_height;
	scroll = new Scrollbar(
		480 - MAP_T_HEIGHT - MAP_BASE_H,
		scroll_content_height,
		MAP_F_WIDTH,
		MAP_BASE_H + MAP_T_HEIGHT,
		16,
		480 - MAP_T_HEIGHT - MAP_BASE_H
	);
}
