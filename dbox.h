#ifndef DBOX_O
#define DBOX_O

#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>


#include <SFML/Graphics.hpp>

#include "game.h"
#include "txmap.h"

#define ATS_DIR "./ats/"

using namespace std;

class DBox : public sf::Drawable, public Game_State
{
private:
	sf::Sprite sp;
	sf::Font font;
	sf::Text text;
	queue<string> lines;

	int text_width;
	int text_height;
	int text_y;
	int text_x;

	int box_width;
	int box_height;
	int box_y;
	int box_x;

	bool prev_paused;

	void draw(sf::RenderTarget& w, sf::RenderStates states) const;

	void init();

public:
	DBox** ptr;
	DBox(DBox** p, string file);
	void fillBox();
	void setText(string msg);
	void update(sf::Event e);
};

#endif
