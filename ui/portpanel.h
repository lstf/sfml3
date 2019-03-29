#ifdef EDITOR_BUILD
#ifndef _PORTPANEL_H
#define _PORTPANEL_H

#include <SFML/Graphics.hpp>

#include "button.h"
#include "textbox.h"
#include "uiutils.h"
#include "../game.h"
#include "../logger.h"
#include "../window.h"
#include "../portals/portal.h"
#include "../portals/portutils.h"
#include "../utils/txmap.h"
#include "../utils/sfutils.h"

#define PORT_BASE_H	16
#define PORT_B_H	16
#define PORT_B_W	64
#define PORT_TOP_H	(2 * PORT_B_H + 16)
#define PORT_TOP_W	(128 + 16)
#define PORT_BOT_H	(480 - PORT_TOP_H - PORT_BASE_H)
#define PORT_BOT_W	(128 + 16)

#define PORT_B_COUNT	1
#define PORT_B_DOOR		0

#define PORTAL_X	0
#define PORTAL_Y	1
#define PORTAL_NAME	2

struct PortButton {
	Button* btn;
	string name;
};

class Portpanel : public sf::Drawable {
public:
	Portpanel(Game* _game, SnapVals* _sv);

	void handle_input(
	sf::Event &event, sf::Vector2i m_pos, sf::Vector2f w_pos);

	void reset(bool retain_sel = false);

	~Portpanel();

private:
	Game* game;
	Map* map;
	SnapVals* sv;

	sf::RectangleShape top_bg;
	sf::RectangleShape bottom_bg;
	sf::Text text;

	Textbox* port_tb;
	bool typing;
	int field;

	Button* name_b;
	Button* x_b;
	Button* y_b;
	PortButton* buttons;

	bool selected;
	PortalSpawner* spawn;

	void button_setup();

	virtual void draw(sf::RenderTarget& w, sf::RenderStates states) const;
};

#endif
#endif
