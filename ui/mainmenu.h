#ifndef _MAINMENU_H
#define _MAINMENU_H

#include <string>
#include <vector>

#include <SFML/Graphics.hpp>

#include "uiutils.h"
#include "../game.h"

#define MAINMENU_BUTTONS 4

#define MMNU_BG sf::Color::Black
#define MMNU_FG sf::Color::White

enum MainMenuSelection {
	MMS_NONE,
	MMS_NEW_GAME,
	MMS_CONTINUE,
	MMS_OPTIONS,
	MMS_QUIT
};

class MainMenu : public sf::Drawable {
private:
	Game* game;

	sf::RectangleShape bg;

	MenuButton buttons[MAINMENU_BUTTONS];
	int menu_selection;

	void select(int i);

	virtual void draw(sf::RenderTarget& w, sf::RenderStates states) const;

public:
	MainMenuSelection handle_input(sf::Event &event);
	
	MainMenu(Game* _game);
};


#endif
