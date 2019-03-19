#ifndef _ENTPANEL_H
#define _ENTPANEL_H

#include <iostream>

#include <SFML/Graphics.hpp>

#include "button.h"
#include "textbox.h"
#include "uiutils.h"
#include "../game.h"
#include "../window.h"
#include "../entities/entity.h"
#include "../entities/keylock.h"
#include "../entities/itement.h"
#include "../utils/txmap.h"
#include "../utils/sfutils.h"

#define ENT_BASE_H 16
#define ENT_B_H 16
#define ENT_B_W 64
#define ENT_TOP_H 128
#define ENT_TOP_W (128 + 16)
#define ENT_BOT_H (480 - ENT_TOP_H - ENT_BASE_H)
#define ENT_BOT_W ENT_TOP_W

#define ENT_BG sf::Color(127,127,127)
#define ENT_FG sf::Color(0,0,0)

#define ENT_B_COUNT 2
#define ENT_B_ITEM 0
#define ENT_B_KEYLOCK 1

struct EntButton {
	Button* btn;
	string name;
};

enum ENT_TYPE {
	ENTT_ITEM,
	ENTT_KEYLOCK
};

class Entpanel : public sf::Drawable {
private:
	Game* game;
	Map* map;
	SnapVals* sv;

	ItemEntUI* itui;
	KeyLockUI* klui;

	sf::RectangleShape top_bg;
	sf::RectangleShape bottom_bg;
	
	ENT_TYPE et;

	sf::Text text;
	EntButton* buttons;

	bool selected;
	Entity* active_ent;

	void button_setup();
	void key_setup();

	virtual void draw(sf::RenderTarget& w, sf::RenderStates states) const;

	void draw_key(sf::RenderTarget& w, sf::RenderStates states) const;

	bool handle_key_input(sf::Event &event, sf::Vector2i m_pos);

public:
	void handle_input(sf::Event &event, sf::Vector2i m_pos,
	sf::Vector2f w_pos);

	void reset(bool retain_sel = false);

	Entpanel(Game* _game, SnapVals* _sv);
};

#endif
