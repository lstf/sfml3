#ifndef _SNAPPANEL_H
#define _SNAPPANEL_H

#include <string>

#include <SFML/Graphics.hpp>

#include "button.h"
#include "textbox.h"
#include "uiutils.h"
#include "../window.h"
#include "../utils/sfutils.h"

#define SNAP_BASE_H 16
#define SNAP_WIDTH 128 + 16
#define SNAP_HEIGHT 48
#define SNAP_B_WIDTH 64
#define SNAP_BG sf::Color(127,127,127)
#define SNAP_FG sf::Color(0,0,0)

enum SNAP_FIELD {
	SNAP_VALUE_X,
	SNAP_OFFSET_X,
	SNAP_VALUE_Y,
	SNAP_OFFSET_Y
};

class Snappanel : public sf::Drawable {
private:
	SnapVals* sv;

	sf::RectangleShape bg;

	Textbox*	snap_tb;
	bool		typing;
	SNAP_FIELD	sf;

	Button* snap_x_b;
	Button* snap_xo_b;
	Button* snap_y_b;
	Button* snap_yo_b;

	virtual void draw(sf::RenderTarget& w, sf::RenderStates states) const;

public:
	void handle_input(sf::Event &event, sf::Vector2i m_pos);

	void reset();
	
	Snappanel(SnapVals* _sv);
};

#endif
