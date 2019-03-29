#ifdef EDITOR_BUILD
#ifndef _TEXTBOX_H
#define _TEXTBOX_H

#include <iostream>
#include <string>

#include <SFML/Graphics.hpp>

#include "../utils/txmap.h"

#define TEXTBOX_BLINK_FPS 15.0
#define TEXTBOX_BLINK_TIME (1 / TEXTBOX_BLINK_FPS)
#define TEXTBOX_BACKSPACE_FPS 8.0
#define TEXTBOX_BACKSPACE_TIME (1 / TEXTBOX_BACKSPACE_FPS)

using namespace std;

class Textbox : public sf::Drawable {
private:
	string left;
	string cursor;
	string right;

	sf::RectangleShape body;

	sf::Text text;

	bool digits;

	sf::Clock clock;
	float time;

	bool backspace;
	float backspace_time;

	virtual void draw(sf::RenderTarget& w, sf::RenderStates states) const;

public:
	string* handle_input(sf::Event &event);

	void update();

	Textbox(sf::FloatRect r, bool _digits = false, string t = "");
};

#endif
#endif
