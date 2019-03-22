#ifndef _RECTPANEL_H
#define _RECTPANEL_H

#include <iostream>
#include <sstream>
#include <vector>

#include <SFML/Graphics.hpp>

#include "button.h"
#include "scrollbar.h"
#include "uiutils.h"
#include "../window.h"

#define RECT_BASE_H 16
#define RECT_W 128
#define RECT_S_W 16
#define RECT_S_H (480 - RECT_BASE_H)
#define RECT_B_H 16

using namespace std;

struct Rectbutton {
	Button* btn;
	sf::FloatRect* r;
};

class Rectpanel : public sf::Drawable {
private:
	SnapVals* sv;

	sf::RectangleShape bg;

	Scrollbar*	scroll;
	int			scroll_max;
	Rectbutton*	buttons;
	int			button_count;

	sf::View* ret_view;

	sf::RectangleShape add_r;
	vector<sf::FloatRect> rects;
	bool right_click;

	virtual void draw(sf::RenderTarget& w, sf::RenderStates states) const;

public:
	void drawRects(sf::RenderTarget& w, sf::RenderStates states) const;

	void handle_input(sf::Event &event, sf::Vector2i m_pos,
	sf::Vector2f w_pos);

	sf::View* update();

	void reset();

	Rectpanel(SnapVals* _sv);
};

#endif
