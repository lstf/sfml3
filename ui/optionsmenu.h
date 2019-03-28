#ifndef _OPTIONSMENU_H
#define _OPTIONSMENU_H

#include <string>
#include <vector>

#include <SFML/Graphics.hpp>

#include "../logger.h"
#include "../utils/txmap.h"
#include "uiutils.h"

#define OPTIONSMENU_BUTTONS 3

#define OMNU_BG sf::Color::Black
#define OMNU_FG sf::Color::White

enum OptionsMenuSelection {
	OMS_NONE,
	OMS_FULLSCREEN,
	OMS_FS_RES,
	OMS_BACK
};

class OptionsMenu : public sf::Drawable {
private:
	bool fullscreen;
	int mode_index;
	sf::RenderWindow* w;

	sf::RectangleShape bg;

	vector<sf::VideoMode> modes;

	MenuButton buttons[OPTIONSMENU_BUTTONS];
	int menu_selection;

	void set_mode();

	void select(int i);

	virtual void draw(sf::RenderTarget& w, sf::RenderStates states) const;

public:
	OptionsMenuSelection handle_input(sf::Event &event);

	OptionsMenu(sf::RenderWindow* _w);
};

#endif
