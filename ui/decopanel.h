#ifndef _DECO_PANEL
#define _DECO_PANEL

#include <iostream>
#include <string>

#include <SFML/Graphics.hpp>

#include "button.h"
#include "scrollbar.h"

#include "../map/map.h"

#define DECO_BASE_H 16
#define DECO_P_SIZE 128
#define DECO_B_SIZE 64

#define DECO_BG sf::Color(127,127,127)
#define DECO_FG sf::Color(0,0,0)

struct Decobutton {
	Button* btn;
	string file;
};

class Decopanel : public sf::Drawable {
private:
	sf::RenderWindow* window;
	sf::RectangleShape preview_bg;
	sf::Sprite preview_sp;
	sf::RectangleShape file_bg;
	sf::RectangleShape menu_bg;
	Decobutton* buttons;
	Scrollbar* scroll;
	sf::View scrollview;
	int scroll_max;
	unsigned int button_count;
	Map* map;

	virtual void draw(sf::RenderTarget& w, sf::RenderStates states) const;

public:
	void handle_input(sf::Event &event);

	Decopanel(sf::RenderWindow* _w, Map* _map);
	
};

#endif
