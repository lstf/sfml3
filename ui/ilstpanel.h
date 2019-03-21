#ifndef _ILSTPANEL_H
#define _ILSTPANEL_H

#include <iostream>
#include <string>
#include <vector>

#include <SFML/Graphics.hpp>

#include "button.h"
#include "scrollbar.h"
#include "uiutils.h"
#include "../game.h"
#include "../window.h"
#include "../map/map.h"

#define ILST_BASE_H 16
#define ILST_W 128
#define ILST_S_W 16
#define ILST_PLUS_W (128 + ILST_S_W)
#define ILST_PLUS_H 16
#define ILST_S_H (480 - ILST_BASE_H - ILST_PLUS_H)
#define ILST_B_E_W 96
#define ILST_B_V_W 32
#define ILST_B_H 16
#define ILST_BG sf::Color(127,127,127)
#define ILST_FG sf::Color(0,0,0)

using namespace std;

struct Ilstbutton {
	Button* event_b;
	Button* val_b;
	string name;
	int val;
};

class Ilstpanel : public sf::Drawable {
private:
	Game* game;
	Map* map;
	sf::RectangleShape bg;

	Button* plus_b;

	Scrollbar*	scroll;
	int			scroll_max;
	Ilstbutton*	buttons;
	int			button_count;

	Textbox* tb;
	string event_name;
	int	event_val;
	int field;
	bool typing;

	void cleanup();

	virtual void draw(sf::RenderTarget& w, sf::RenderStates states) const;

public:
	Ilstpanel(Game* _game);

	void handle_input(sf::Event &event, sf::Vector2i m_pos);

	void reset();

	~Ilstpanel();
};

#endif
