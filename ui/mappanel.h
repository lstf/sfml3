#ifndef _MAPPANEL_H
#define _MAPPANEL_H

#include <iostream>
#include <string>

#include <SFML/Graphics.hpp>

#include "button.h"
#include "scrollbar.h"
#include "textbox.h"
#include "../game.h"
#include "../window.h"
#include "../utils/sfutils.h"

#define MAP_BASE_H 16
#define MAP_F_WIDTH 128
#define MAP_B_WIDTH 64
#define MAP_B_H 16
#define MAP_S_WIDTH 16
#define MAP_T_HEIGHT 128
#define MAP_T_WIDTH MAP_F_WIDTH + MAP_S_WIDTH

#define MAP_BG sf::Color(127,127,127)
#define MAP_FG sf::Color(0,0,0)

using namespace std;

struct MappanelTrans {
	string name;
	bool is_new;
};

enum MAP_NAMING_STATE {
	MAP_NAME_NEW,
	MAP_NAME_DUP,
	MAP_NAME_REN
};

struct Mapbutton {
	Button* btn;
	string name;
};

class Mappanel : public sf::Drawable {
private:
	Game* game;

	sf::RectangleShape top_bg;
	sf::RectangleShape bottom_bg;

	sf::Text	selected_text;
	int			selected;

	Button*	top_buttons[7];
	bool	confirm;
	bool	load_level;
	bool	new_level;
	string	new_level_name;
	bool	delete_level;

	Textbox*			name_tb;
	bool				naming;
	MAP_NAMING_STATE	nstate;

	Scrollbar*		scroll;
	int				scroll_max;
	Mapbutton*		buttons;
	unsigned int	button_count;

	void bg_setup();
	void button_setup();
	void scroll_setup();

	virtual void draw(sf::RenderTarget& w, sf::RenderStates states) const;

public:
	void handle_input(sf::Event &event, sf::Vector2i m_pos);
	
	MappanelTrans* update();

	void reset();

	Mappanel(Game* _game);
};

#endif
