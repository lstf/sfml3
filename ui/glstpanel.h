#ifdef EDITOR_BUILD
#ifndef _GLSTPANEL_H
#define _GLSTPANEL_H

#include <iostream>
#include <string>
#include <vector>

#include <SFML/Graphics.hpp>

#include "button.h"
#include "scrollbar.h"
#include "uiutils.h"
#include "../game.h"
#include "../window.h"

#define GLST_BASE_H 16
#define GLST_W 128
#define GLST_S_W 16
#define GLST_PLUS_W (128 + GLST_S_W)
#define GLST_PLUS_H 16
#define GLST_S_H (480 - GLST_BASE_H - GLST_PLUS_H)
#define GLST_B_E_W 96
#define GLST_B_V_W 32
#define GLST_B_H 16

using namespace std;

struct Glstbutton {
	Button* event_b;
	Button* val_b;
	string name;
	int val;
};

class Glstpanel : public sf::Drawable {
private:
	Game* game;
	sf::RectangleShape bg;

	Button* refresh_b;

	sf::View	scrollview;
	Scrollbar*	scroll;
	int			scroll_max;
	Glstbutton*	buttons;
	int			button_count;

	Textbox* tb;
	string event_name;
	int	event_val;
	int field;
	bool typing;

	void cleanup();

	virtual void draw(sf::RenderTarget& w, sf::RenderStates states) const;

public:
	Glstpanel(Game* _game);

	void handle_input(sf::Event &event, sf::Vector2i m_pos);

	void reset();

	~Glstpanel();
};

#endif
#endif
