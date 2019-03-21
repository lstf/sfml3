#include "mainmenu.h"

void MainMenu::draw(sf::RenderTarget& w, sf::RenderStates states) const {
	if (options_active) {
		w.draw(*options, states);
		return;
	}

	for (int i = 0; i < MAINMENU_BUTTONS; i++) {
		w.draw(buttons[i].body, states);
		w.draw(buttons[i].text, states);
	}
}


MainMenuSelection MainMenu::handle_input(sf::Event &event) {
	if (options_active) {
		OptionsMenuSelection oms = options->handle_input(event);
		if (oms == OMS_BACK) {
			options_active = false;
		}
		return MMS_NONE;
	}

	if (event.type == sf::Event::KeyPressed) {
		if (event.key.code == sf::Keyboard::Down) {
			if (menu_selection < MAINMENU_BUTTONS - 1) {
				menu_selection++;
				select(menu_selection);
			}
		} else if (event.key.code == sf::Keyboard::Up) {
			if (menu_selection > 0) {
				menu_selection--;
				select(menu_selection);
			}
		} else if (event.key.code == sf::Keyboard::Enter) {
			switch (menu_selection + 1) {
			case MMS_NEW_GAME:
				return MMS_NEW_GAME;
			case MMS_CONTINUE:
				return MMS_CONTINUE;
			case MMS_OPTIONS:
				options_active = true;
				return MMS_NONE;
			case MMS_QUIT:
				return MMS_QUIT;
			default:
				return MMS_NONE;
			}
		}
	}
	return MMS_NONE;
}


void MainMenu::select(int i) {
	for (int j = 0; j < MAINMENU_BUTTONS; j++) {
		if (j == i) {
			buttons[j].body.setOutlineColor(MMNU_FG);
		} else {
			buttons[j].body.setOutlineColor(MMNU_BG);
		}
	}
}

void gen_buttons(int i, MenuButton &btn) {
	btn.body.setSize(sf::Vector2f(480, 32));
	btn.body.setPosition(240, i * 48 + 128);
	btn.body.setFillColor(MMNU_BG);
	btn.body.setOutlineThickness(-1.0);
	btn.body.setOutlineColor(MMNU_BG);
	btn.text.setFont(*txmap::get_font("./ats/fonts/thintel.ttf"));
	btn.text.setCharacterSize(24);
	btn.text.setFillColor(MMNU_FG);
	switch (i+1) {
	case MMS_NEW_GAME:
		btn.text.setString("new game");
		break;
	case MMS_CONTINUE:
		btn.text.setString("continue");
		break;
	case MMS_OPTIONS:
		btn.text.setString("options");
		break;
	case MMS_QUIT:
		btn.text.setString("quit");
		break;
	default:
		btn.text.setString("error");
	}
	sf::Vector2f tp = centerIn(
			btn.text.getGlobalBounds(),
			btn.body.getGlobalBounds()
	);
	btn.text.setPosition((int)tp.x, i * 48 - 4 + 128);
}

MainMenu::MainMenu(Game* _game, sf::RenderWindow* w) {
	game = _game;

	bg.setSize(sf::Vector2f(960, 480));
	bg.setPosition(0, 0);
	bg.setFillColor(MMNU_BG);

	for (int i = 0; i < MAINMENU_BUTTONS; i++) {
		gen_buttons(i, buttons[i]);
	}
	select(0);
	options = new OptionsMenu(w);
	options_active = false;
}

