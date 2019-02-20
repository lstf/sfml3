#ifndef _TEXTBOX_O
#define _TEXTBOX_O

#include <iostream>
#include <string>

#include <SFML/Graphics.hpp>

#include "../utils/txmap.h"

using namespace std;

class Textbox : public sf::Drawable {
private:
	string left;
	string right;
	sf::Text text;
	sf::RectangleShape body;
	virtual void draw(sf::RenderTarget& w, sf::RenderStates states) const;

public:
	string* handle_input(sf::Event &event);
	Textbox(sf::FloatRect r, string t = "");
};
#endif
