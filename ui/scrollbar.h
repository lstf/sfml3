#ifndef _SCROLLBAR_H
#define _SCROLLBAR_H

#include <SFML/Graphics.hpp>

#include "../utils/sfutils.h"

#define SCROLL_FG sf::Color(127,127,127)
#define SCROLL_BG sf::Color(0,0,0)

using namespace std;

class Scrollbar : public sf::Drawable {
private:
	sf::RectangleShape bg;
	sf::RectangleShape fg;

	bool			clicked;
	sf::Vector2i	p_m_pos;

	int max_y;
	int cur_y;

	virtual void draw(sf::RenderTarget& w, sf::RenderStates states) const;

public:
	float handle_input(sf::Event &event, sf::Vector2i m_pos);

	Scrollbar(int h1, int h2, int x, int y, int w, int h);
};

#endif 
