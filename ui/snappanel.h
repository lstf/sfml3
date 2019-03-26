#ifndef _SNAPPANEL_H
#define _SNAPPANEL_H

#include <string>
#include <vector>

#include <SFML/Graphics.hpp>

#include "button.h"
#include "scrollbar.h"
#include "textbox.h"
#include "uiutils.h"
#include "../window.h"
#include "../utils/sfutils.h"

#define SNAP_BASE_H 16
#define SNAP_T_W (128 + 16)
#define SNAP_T_H 128
#define SNAP_BG_W 128
#define SNAP_BG_H (480 - SNAP_BASE_H - SNAP_T_H)
#define SNAP_B_W 64
#define SNAP_S_W 16
#define SNAP_S_H SNAP_BG_H

struct SnapButton {
	Button* btn;
	int i;
};

enum SNAP_FIELD {
	SNAP_VALUE_X,
	SNAP_OFFSET_X,
	SNAP_VALUE_Y,
	SNAP_OFFSET_Y
};

class Snappanel : public sf::Drawable {
private:
	SnapVals* sv;

	sf::RectangleShape top_bg;
	sf::RectangleShape bottom_bg;

	Textbox*	snap_tb;
	bool		typing;
	SNAP_FIELD	sf;

	Button* snap_x_b;
	Button* snap_xo_b;
	Button* snap_y_b;
	Button* snap_yo_b;
	Button* add_b;


	vector<SnapVals> svs;
	Scrollbar* scroll;
	int scroll_max;
	SnapButton* buttons;
	int button_count;

	void scroll_setup();

	virtual void draw(sf::RenderTarget& w, sf::RenderStates states) const;

public:
	void handle_input(sf::Event &event, sf::Vector2i m_pos);

	void reset();
	
	Snappanel(SnapVals* _sv);
};

#endif
