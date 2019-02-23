#ifndef _MAPPANEL_O
#define _MAPPANEL_O

#include <iostream>
#include <string>

#include <SFML/Graphics.hpp>

#include "button.h"
#include "scrollbar.h"
#include "textbox.h"
#include "../game.h"
#include "../utils/sfutils.h"

using namespace std;

#define MAP_BASE_H 16
#define MAP_F_WIDTH 128
#define MAP_B_WIDTH 64
#define MAP_S_WIDTH 16
#define MAP_T_HEIGHT 128
#define MAP_T_WIDTH MAP_F_WIDTH + MAP_S_WIDTH

#define MAP_BG sf::Color(127,127,127)
#define MAP_FG sf::Color(0,0,0)

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
	sf::RectangleShape top_bg;
	sf::RectangleShape bottom_bg;

	Button* top_buttons[7];
	int selected;
	sf::Text selected_text;
	bool confirm;
	Textbox* name_tb;
	bool naming;
	MAP_NAMING_STATE nstate;
	bool load_level;
	bool new_level;
	string new_level_name;

	sf::View scrollview;
	Scrollbar* scroll;
	int scroll_max;
	Mapbutton* buttons;
	unsigned int button_count;

	Game* game;

	void bg_setup();
	void button_setup();
	void scroll_setup();

	virtual void draw(sf::RenderTarget& w, sf::RenderStates states) const;

public:
	MappanelTrans* update();

	void handle_input(sf::Event &event, sf::Vector2i m_pos);

	void reset();

	Mappanel(Game* _game);
};

#endif
