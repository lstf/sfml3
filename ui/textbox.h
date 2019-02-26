#ifndef _TEXTBOX_H
#define _TEXTBOX_H

#include <iostream>
#include <string>

#include <SFML/Graphics.hpp>

#include "../utils/txmap.h"

#define TEXTBOX_BLINK_FRAMES 15

using namespace std;

class Textbox : public sf::Drawable {
private:
	string left;
	string cursor;
	string right;

	sf::RectangleShape body;

	sf::Text text;

	bool digits;

	int blink_frame;

	virtual void draw(sf::RenderTarget& w, sf::RenderStates states) const;

public:
	string* handle_input(sf::Event &event);

	void update();

	Textbox(sf::FloatRect r, bool _digits = false, string t = "");
};
#endif
