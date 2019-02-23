#ifndef _TEXTBOX_O
#define _TEXTBOX_O

#include <iostream>
#include <string>

#include <SFML/Graphics.hpp>

#include "../utils/txmap.h"

#define TEXTBOX_BLINK_FRAMES 15

using namespace std;

class Textbox : public sf::Drawable {
private:
	string left;
	string right;
	sf::Text text;
	sf::RectangleShape body;
	sf::String cursor;
	bool digits;
	int blink_frame;
	virtual void draw(sf::RenderTarget& w, sf::RenderStates states) const;

public:
	void update();
	string* handle_input(sf::Event &event);
	Textbox(sf::FloatRect r, bool dig = false, string t = "");
};
#endif
