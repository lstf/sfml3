#ifndef _PORTPANEL_H
#define _PORTPANEL_H

#include <SFML/Graphics.hpp>

#include "button.h"
#include "textbox.h"
#include "uiutils.h"
#include "../game.h"
#include "../actors/portal.h"
#include "../utils/txmap.h"
#include "../utils/sfutils.h"

#define PORT_BASE_H 16
#define PORT_B_H 16
#define PORT_B_W 64
#define PORT_TOP_H (2 * PORT_B_H + 16)
#define PORT_TOP_W (128 + 16)
#define PORT_BOT_H (480 - PORT_TOP_H - PORT_BASE_H)
#define PORT_BOT_W (128 + 16)
#define PORT_BG sf::Color(127,127,127)
#define PORT_FG sf::Color(0,0,0)

#define PORT_B_COUNT 1
#define PORT_B_DOOR 0

struct PortButton {
	Button* btn;
	string name;
};

enum PORT_FIELD {
	PORTAL_X,
	PORTAL_Y,
	PORTAL_NAME
};

class Portpanel : public sf::Drawable {
private:
	Game* game;
	Map* map;
	SnapVals* sv;

	sf::RectangleShape top_bg;
	sf::RectangleShape bottom_bg;

	Textbox* port_tb;
	bool typing;
	PORT_FIELD pf;

	Button* name_b;
	sf::Text text;
	Button* x_b;
	Button* y_b;
	PortButton* buttons;

	bool selected;
	Portal* active_por;

	void button_setup();

	virtual void draw(sf::RenderTarget& w, sf::RenderStates states) const;

public:
	void handle_input(
	sf::Event &event, sf::Vector2i m_pos, sf::Vector2f w_pos);

	void reset(bool retain_sel = false);

	Portpanel(Game* _game, SnapVals* _sv);
};

#endif
