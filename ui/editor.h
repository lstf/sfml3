#ifndef _EDITOR_H
#define _EDITOR_H

#include <vector>
#include <string>
#include <sstream>

#include <SFML/Graphics.hpp>

#include "button.h"
#include "decopanel.h"
#include "mappanel.h"
#include "geompanel.h"
#include "portpanel.h"
#include "rectpanel.h"
#include "snappanel.h"
#include "entpanel.h"
#include "ilstpanel.h"
#include "glstpanel.h"
#include "../game.h"
#include "../player.h"
#include "../entities/sign.h"
#include "../enemies/testenemy.h"
#include "../map/map.h"
#include "../utils/sfutils.h"

struct EditorTrans {
	string name;
	sf::Vector2f position;
	bool is_new;
};

enum Modes {
	EDIT_DECO,
	EDIT_GEOM,
	EDIT_MAP,
	EDIT_SNAP,
	EDIT_PORT,
	EDIT_RECT,
	EDIT_ENT,
	EDIT_ILST,
	EDIT_GLST
};

enum RootButtons {
	MAP_BUTTON,
	SNAP_BUTTON,
	DECO_BUTTON,
	GEOM_BUTTON,
	POR_BUTTON,
	ENT_BUTTON,
	ENM_BUTTON,
	RECT_BUTTON,
	ILST_BUTTON,
	GLST_BUTTON
};

class Editor: public sf::Drawable {
private:
	sf::RenderWindow* w;
	Game* game;

	Modes mode;

	Button*				buttons[15];
	sf::RectangleShape	selected_pan_r;

	Decopanel*	decopanel;
	Entpanel*	entpanel;
	Mappanel*	mappanel;
	Geompanel*	geompanel;
	Glstpanel*	glstpanel;
	Ilstpanel*	ilstpanel;
	Portpanel*	portpanel;

	Rectpanel*	rectpanel;
	bool		rectpanel_rects;

	Snappanel*	snappanel;
	bool 		snap;
	SnapVals	sv;
	SnapVals	sv_b;

	sf::Vector2i	p_m_pos;
	bool			mouse_middle;

	virtual void draw(sf::RenderTarget& w, sf::RenderStates states) const;

	Button* gen_button(int i);

public:
	sf::View view;

	void handle_input(sf::Event &event);

	EditorTrans* update();

	void reset();

	Editor(sf::RenderWindow* _w, Game* _game);
};

#endif
