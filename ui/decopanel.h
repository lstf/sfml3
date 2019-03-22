#ifndef _DECOPANEL_H
#define _DECOPANEL_H

#include <iostream>
#include <string>

#include <SFML/Graphics.hpp>

#include "button.h"
#include "scrollbar.h"
#include "uiutils.h"
#include "../game.h"
#include "../map/map.h"
#include "../window.h"

#define DECO_BASE_H 16
#define DECO_P_SIZE 128
#define DECO_B_SIZE 64
#define DECO_B_H 16

enum DECO_STATE {
	DECO_EDIT,
	DECO_ADD,
	DECO_REMOVE,
	DECO_SWAP
};

struct Decobutton {
	Button* btn;
	string file;
};

class Decopanel : public sf::Drawable {
private:
	Game* game;
	Map* map;
	SnapVals* sv;

	sf::RectangleShape file_bg;
	sf::RectangleShape menu_bg;
	sf::RectangleShape preview_bg;

	sf::Sprite preview_sp;

	Scrollbar*		scroll;
	int				scroll_max;
	Decobutton*		buttons;
	unsigned int	button_count;

	sf::Sprite	active_sp;
	string		active_name;
	int			active_layer;
	Button*		layer_toggle;
	Button*		layer_up;
	Button*		layer_down;

	sf::RectangleShape	select_r;
	bool				select_click;
	bool				selected_click;
	sf::Vector2f		select_p_mouse;
	vector<int>			selected;

	DECO_STATE state;

	sf::IntRect bounds;

	virtual void draw(sf::RenderTarget& w, sf::RenderStates states) const;

	void bg_setup();
	void button_setup();
	void scroll_setup();
	void select_setup();
	vector<sf::FloatRect> gen_geom();

public:
	void handle_input(
	sf::Event &event, sf::Vector2i m_pos, sf::Vector2f w_pos);

	void reset();

	Decopanel(Game* _game, SnapVals* _sv);
};

#endif
