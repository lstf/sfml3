#include "optionsmenu.h"

string mode_string(sf::VideoMode m) {
	return to_string(m.width) + "x" + to_string(m.height);
}

void OptionsMenu::set_mode() {
	if (fullscreen) {
		w->create(modes[mode_index], "Game", sf::Style::Fullscreen);
	} else {
		w->create(sf::VideoMode(960, 480), "Game");
	}
	w->setVerticalSyncEnabled(true);
	w->setKeyRepeatEnabled(false);
}

void OptionsMenu::select(int i) {
	for (int j = 0; j < OPTIONSMENU_BUTTONS; j++) {
		if (j == i) {
			buttons[j].body.setOutlineColor(OMNU_FG);
		} else {
			buttons[j].body.setOutlineColor(OMNU_BG);
		}
	}
}

void OptionsMenu::draw(sf::RenderTarget& w, sf::RenderStates states) const{
	for (int i = 0; i < OPTIONSMENU_BUTTONS; i++) {
		w.draw(buttons[i].body, states);
		w.draw(buttons[i].text, states);
	}
}

void gen_buttons(int i, MenuButton &btn, string str) {
	btn.body.setSize(sf::Vector2f(480, 32));
	btn.body.setPosition(240, i * 48 + 128);
	btn.body.setFillColor(OMNU_BG);
	btn.body.setOutlineThickness(-1.0);
	btn.body.setOutlineColor(OMNU_BG);
	btn.text.setFont(*txmap::get_font("./ats/fonts/thintel.ttf"));
	btn.text.setCharacterSize(24);
	btn.text.setFillColor(OMNU_FG);
	btn.text.setString(str);
	sf::Vector2f tp = centerIn(
			btn.text.getGlobalBounds(),
			btn.body.getGlobalBounds()
	);
	btn.text.setPosition((int)tp.x, i * 48 - 4 + 128);
}

OptionsMenuSelection OptionsMenu::handle_input(sf::Event &event) {
	if (event.type == sf::Event::KeyPressed) {
		if (event.key.code == sf::Keyboard::Down) {
			if (menu_selection < OPTIONSMENU_BUTTONS - 1) {
				menu_selection++;
				select(menu_selection);
			}
		} else if (event.key.code == sf::Keyboard::Up) {
			if (menu_selection > 0) {
				menu_selection--;
				select(menu_selection);
			}
		} else if (event.key.code == sf::Keyboard::Left ||
		event.key.code == sf::Keyboard::Right) {
			if (menu_selection == 0) {
				if (fullscreen) {
					fullscreen = false;
					gen_buttons(0, buttons[0], "Fullscreen: off");
					set_mode();
				} else {
					fullscreen = true;
					gen_buttons(0, buttons[0], "Fullscreen: on");
					set_mode();
				}
			} else if (menu_selection == 1) {
				if (event.key.code == sf::Keyboard::Right) {
					if (mode_index < (int)modes.size() - 1) {
						mode_index++;
						gen_buttons(1, buttons[1], "Mode: " + 
						mode_string(modes[mode_index]));
						if (fullscreen) {
							set_mode();
						}
					}
				} else {
					if (mode_index > 0) {
						mode_index--;
						gen_buttons(1, buttons[1], "Mode: " + 
						mode_string(modes[mode_index]));
						if (fullscreen) {
							set_mode();
						}
					}
				}
			}
			select(menu_selection);
		} else if (event.key.code == sf::Keyboard::Enter) {
			switch (menu_selection + 1) {
			case OMS_BACK:
				return OMS_BACK;
			default:
				return OMS_NONE;
			}
		}
	}
	return OMS_NONE;
}


OptionsMenu::OptionsMenu(sf::RenderWindow* _w) {
	log_dbg("constructing options menu");
	w = _w;

	vector<sf::VideoMode> modes_temp = sf::VideoMode::getFullscreenModes();

	for (int i = 0; i < (int)modes_temp.size(); i++) {
		int m_w = modes_temp[i].width;
		int m_h = modes_temp[i].height;
		int bpp = modes_temp[i].bitsPerPixel;
		if (m_w >= 960 && m_h >= 480 && bpp == 32) {
			modes.push_back(modes_temp[i]);
		}
	}
	mode_index = 0;

	fullscreen = false;

	bg.setSize(sf::Vector2f(960, 480));
	bg.setPosition(0, 0);
	bg.setFillColor(OMNU_BG);
	gen_buttons(0, buttons[0], "Fullscreen: off");
	gen_buttons(1, buttons[1], "Mode: " + mode_string(modes[mode_index]));
	gen_buttons(3, buttons[2], "Back");
	menu_selection = 0;
	select(0);
}
